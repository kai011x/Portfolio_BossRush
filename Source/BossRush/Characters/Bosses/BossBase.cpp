// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Bosses/BossBase.h"
#include "GimmickManager.h"
#include "Kismet/GameplayStatics.h"
#include "AbilitySystemComponent.h"
#include "GAS/AttributeSets/BossAttributeSet.h"
#include "Characters/Bosses/BossAIController.h"
#include "GimmickBase.h"
#include "GAS/Abilities/Boss/GA_PatternBase.h"
#include "Components/StateTreeComponent.h"
#include "TimerManager.h"
#include "Engine/World.h"

ABossBase::ABossBase()
{
	// 보스 기본 설정
	Identity = EIdentity::Boss;
	CharacterSize = ESize::Big;

	// AI 컨트롤러 설정
	AIControllerClass = ABossAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// 보스 전용 어트리뷰트 세트 설정
	AttributeSetClass = UBossAttributeSet::StaticClass();

	// StateTree 컴포넌트 생성 및 설정
	StateTreeComponent = CreateDefaultSubobject<UStateTreeComponent>(TEXT("StateTreeComponent"));
}


void ABossBase::BeginPlay()
{
	Super::BeginPlay();

	// 기믹 매니저의 결과 보고 이벤트 구독
	if (UGimmickManager* Manager = UGimmickManager::Get(this))
	{
		Manager->OnGimmickResultReported.AddDynamic(this, &ABossBase::OnGimmickFinished);
	}

	// 어빌리티 종료 이벤트 구독 (패턴 종료 감지용)
	if (GetAbilitySystemComponent())
	{
		GetAbilitySystemComponent()->OnAbilityEnded.AddUObject(this, &ABossBase::OnAbilityEnded);
	}

	// 데이터 테이블 내 거리별 몽타주 개수 집계
	CalculateDistanceMontageCounts();

	// 초기 타겟 설정
	SwitchTarget();
}

void ABossBase::RecordDamage(ACharacterBase* Attacker, float Amount)
{
	if (!Attacker) return;

	// 플레이어별 대미지 기록
	float& TotalDamage = PlayerDamageMap.FindOrAdd(Attacker);
	TotalDamage += Amount;

	// 타겟 전환용 누적 대미지 체크
	DamageAccumulatedSinceLastSwitch += Amount;
	if (DamageAccumulatedSinceLastSwitch >= DamageThresholdForSwitch)
	{
		UE_LOG(LogTemp, Warning, TEXT("[BossBase] Damage threshold reached (%.1f). Switching target."), DamageAccumulatedSinceLastSwitch);
		SwitchTarget();
	}
}

void ABossBase::ExecutePattern()
{
	// 현재 실행 중이거나 타겟이 없으면 실행하지 않음
	if (bIsExecutingPattern || !CurrentTarget) return;

	switch (CurPatternType)
	{
	case EPatternType::Attack:
		AbilitySystemComponent->TryActivateAbilityByClass(NormalAttackAbilityClass);

		break;
	case EPatternType::Dodge:
		break;
	case EPatternType::Defense:
		break;
	case EPatternType::Max:
		break;
	default:
		break;
	}


}

EDistance ABossBase::GetCurrentDistanceKey() const
{
	if (!CurrentTarget) return EDistance::Far;

	float Distance = GetDistanceTo(CurrentTarget);

	if (Distance < 300.0f)
	{
		return EDistance::Near;
	}
	else if (Distance < 1000.0f)
	{
		return EDistance::Middle;
	}

	return EDistance::Far;
}

void ABossBase::CalculateDistanceMontageCounts()
{
	DistanceMontageCounts.Empty();
	DistanceMontageCounts.Add(EDistance::Near, 0);
	DistanceMontageCounts.Add(EDistance::Middle, 0);
	DistanceMontageCounts.Add(EDistance::Far, 0);

	if (BossAttackMontageDataTable)
	{
		TArray<FBossMontageData*> OutRowList;
		BossAttackMontageDataTable->GetAllRows<FBossMontageData>(TEXT(""), OutRowList);

		for (FBossMontageData* RowData : OutRowList)
		{
			if (RowData)
			{
				int32* Count = DistanceMontageCounts.Find(RowData->AttackRange);
				if (Count)
				{
					(*Count)++;
				}
			}
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("[BossBase] Distance Montage Counts - Near: %d, Middle: %d, Far: %d"),
		DistanceMontageCounts[EDistance::Near],
		DistanceMontageCounts[EDistance::Middle],
		DistanceMontageCounts[EDistance::Far]);
}

EPatternType ABossBase::GetNextPattern()
{
	if (IsExecutingPattern()) {
		return CurPatternType; // 이미 패턴 실행 중이면 현재 패턴 반환
	}

	if (!CurrentTarget) return EPatternType::Attack;

	//EDistance SelectedDistKey = GetCurrentDistanceKey();
	EDistance SelectedDistKey = EDistance::Near;
	return EPatternType::Attack;

	//const FPatternTypeList* PatternList = DistancePatternMap.Find(SelectedDistKey);
	//if (PatternList && PatternList->PatternTypes.Num() > 0)
	//{
	//	int32 RandomIdx = FMath::RandRange(0, PatternList->PatternTypes.Num() - 1);
	//	return PatternList->PatternTypes[RandomIdx];
	//}

	//return EPatternType::Max;
}

void ABossBase::RotateToTarget()
{
	if (!CurrentTarget) return;

	FVector Direction = CurrentTarget->GetActorLocation() - GetActorLocation();
	Direction.Z = 0.0f;
	
	if (!Direction.IsNearlyZero())
	{
		FRotator TargetRot = FRotationMatrix::MakeFromX(Direction).Rotator();
		SetActorRotation(TargetRot);
	}
}

void ABossBase::OnAbilityEnded(const FAbilityEndedData& AbilityEndedData)
{
	// 패턴 실행 중일 때 어빌리티가 종료되면 쿨타임 시작
	if (bIsExecutingPattern)
	{
		// 쿨타임 타이머 설정
		GetWorldTimerManager().SetTimer(CooldownTimerHandle, this, &ABossBase::ResetPatternExecution, PatternCooldown, false);
	}
}

void ABossBase::ResetPatternExecution()
{
	bIsExecutingPattern = false;
	UE_LOG(LogTemp, Warning, TEXT("[BossBase] Cooldown finished. Ready for next pattern."));
}

void ABossBase::SwitchTarget()
{
	TArray<ACharacterBase*> ValidPlayers;

	// 월드에 존재하는 플레이어 컨트롤러만 직접 순회하여 캐릭터 수집
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PC = Iterator->Get();
		if (PC)
		{
			ACharacterBase* PlayerChar = Cast<ACharacterBase>(PC->GetPawn());
			if (PlayerChar && PlayerChar->Identity == EIdentity::Player)
			{
				ValidPlayers.Add(PlayerChar);
			}
		}
	}

	if (ValidPlayers.Num() == 0)
	{
		CurrentTarget = nullptr;
		return;
	}

	// 3가지 방식 중 랜덤 선택
	ETargetSwitchMethod Method = (ETargetSwitchMethod)FMath::RandRange(0, 2);
	ACharacterBase* NewTarget = nullptr;

	switch (Method)
	{
	case ETargetSwitchMethod::Random:
	{
		NewTarget = ValidPlayers[FMath::RandRange(0, ValidPlayers.Num() - 1)];
		UE_LOG(LogTemp, Warning, TEXT("[BossBase] Switching Target Method: Random -> %s"), *NewTarget->GetName());
		break;
	}
	case ETargetSwitchMethod::TopDamager:
	{
		float MaxDamage = -1.0f;
		for (ACharacterBase* Player : ValidPlayers)
		{
			float Damage = PlayerDamageMap.FindRef(Player);
			if (Damage > MaxDamage)
			{
				MaxDamage = Damage;
				NewTarget = Player;
			}
		}
		UE_LOG(LogTemp, Warning, TEXT("[BossBase] Switching Target Method: TopDamager -> %s (Damage: %.1f)"), 
			NewTarget ? *NewTarget->GetName() : TEXT("None"), MaxDamage);
		break;
	}
	case ETargetSwitchMethod::FarthestPlayer:
	{
		float MaxDistance = -1.0f;
		for (ACharacterBase* Player : ValidPlayers)
		{
			float Dist = GetDistanceTo(Player);
			if (Dist > MaxDistance)
			{
				MaxDistance = Dist;
				NewTarget = Player;
			}
		}
		UE_LOG(LogTemp, Warning, TEXT("[BossBase] Switching Target Method: Farthest -> %s (Dist: %.1f)"), 
			NewTarget ? *NewTarget->GetName() : TEXT("None"), MaxDistance);
		break;
	}
	}

	if (NewTarget)
	{
		CurrentTarget = NewTarget;
		// 카운터 초기화
		PatternCounter = 0;
		DamageAccumulatedSinceLastSwitch = 0.0f;
	}
}

void ABossBase::RequestGimmick(int32 GimmickIndex)
{
	if (!GimmickClasses.IsValidIndex(GimmickIndex)) return;

	if (UGimmickManager* Manager = UGimmickManager::Get(this))
	{
		Manager->SpawnAndExecuteGimmick(GimmickClasses[GimmickIndex], this);
	}
}

void ABossBase::OnGimmickFinished(AGimmickBase* Gimmick, bool bSuccess)
{
	if (bSuccess)
	{
		UE_LOG(LogTemp, Warning, TEXT("[BossBase] Gimmick Succeeded! Boss might be stunned."));
		// 성공 시 보스 그로기 상태 등 추가 로직 가능
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[BossBase] Gimmick Failed! Players taking penalty."));
	}
}
