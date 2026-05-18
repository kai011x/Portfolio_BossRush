// Fill out your copyright notice in the Description page of Project Settings.


#include "GimmickBase.h"
#include "Characters/CharacterBase.h"
#include "Characters/BossRushPlayerController.h"
#include "GimmickManager.h"
#include "Kismet/GameplayStatics.h"
#include "AbilitySystemComponent.h"
#include "GAS/Tags/GameplayTags.h"

AGimmickBase::AGimmickBase()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true; // 멀티플레이 동기화 활성화
}

void AGimmickBase::BeginPlay()
{
	Super::BeginPlay();
}

void AGimmickBase::StartGimmick()
{
	UE_LOG(LogTemp, Warning, TEXT("[GimmickBase] Gimmick Started: %s (Type: %d)"), *GetName(), (uint8)GimmickType);

	if (TimeLimit > 0.0f)
	{
		GetWorldTimerManager().SetTimer(GimmickTimerHandle, [this]() {
			EndGimmick(false); // 시간 초과 시 실패 처리
		}, TimeLimit, false);
	}

	OnGimmickStarted();
}

void AGimmickBase::EndGimmick(bool bSuccess)
{
	GetWorldTimerManager().ClearTimer(GimmickTimerHandle);

	UE_LOG(LogTemp, Warning, TEXT("[GimmickBase] Gimmick Ended: %s (Success: %s)"), *GetName(), bSuccess ? TEXT("TRUE") : TEXT("FALSE"));

	// 매니저에게 결과 보고
	if (UGimmickManager* Manager = UGimmickManager::Get(this))
	{
		Manager->ReportGimmickResult(this, bSuccess);
	}

	if (!bSuccess)
	{
		ApplyGimmickPenalty();
	}

	OnGimmickEnded(bSuccess);

	// 기믹 종료 후 일정 시간 뒤 액터 파괴
	SetLifeSpan(1.0f);
}

void AGimmickBase::ApplyGimmickPenalty()
{
	UE_LOG(LogTemp, Warning, TEXT("[GimmickBase] Applying Gimmick Penalty..."));

	// 기본적으로 광역 피해 적용
	ApplyRadialPenalty(PenaltyDamage, PenaltyRadius);

	// 예시: 랜덤 한 명에게 강력한 디버프 적용
	ACharacterBase* RandomTarget = GetRandomPlayer();
	if (RandomTarget)
	{
		ApplyDebuffToTarget(RandomTarget, 0.5f, 0.7f, DebuffDuration); // 이속 50%, 체력 70%로 감소
	}
}

void AGimmickBase::ApplyRadialPenalty(float Damage, float Radius)
{
	ACharacterBase* Boss = Cast<ACharacterBase>(OwnerBoss);
	if (!Boss) return;

	Boss->ApplyRadialDamage(Boss->GetActorLocation(), Radius, Damage / 10.0f, EHitType::Heavy);
}

void AGimmickBase::ApplyDebuffToTarget(ACharacterBase* Target, float SpeedMultiplier, float MaxHealthMultiplier, float Duration)
{
	if (!Target || !Target->GetAbilitySystemComponent()) return;

	// TODO: GameplayEffect를 사용하여 능력치 감소 적용
	UE_LOG(LogTemp, Error, TEXT("[GimmickBase] Debuff Applied to %s: Speed x%.2f, MaxHealth x%.2f for %.1fs"), 
		*Target->GetName(), SpeedMultiplier, MaxHealthMultiplier, Duration);
}

ACharacterBase* AGimmickBase::GetRandomPlayer()
{
	TArray<AActor*> FoundPlayers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacterBase::StaticClass(), FoundPlayers);

	if (FoundPlayers.Num() > 0)
	{
		int32 RandomIndex = FMath::RandRange(0, FoundPlayers.Num() - 1);
		return Cast<ACharacterBase>(FoundPlayers[RandomIndex]);
	}

	return nullptr;
}

void AGimmickBase::SetAllPlayersInputBlock(bool bBlock)
{
	TArray<AActor*> FoundPlayers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacterBase::StaticClass(), FoundPlayers);

	for (AActor* PlayerActor : FoundPlayers)
	{
		ACharacterBase* Character = Cast<ACharacterBase>(PlayerActor);
		if (Character)
		{
			// 1. GAS 태그를 통한 입력 차단 (기존 로직)
			if (Character->GetAbilitySystemComponent())
			{
				FGameplayTag BlockTag = FGameplayTags::Get().BlockInputEventTag;
				if (bBlock)
				{
					Character->GetAbilitySystemComponent()->AddLooseGameplayTag(BlockTag);
				}
				else
				{
					Character->GetAbilitySystemComponent()->RemoveLooseGameplayTag(BlockTag);
				}
			}

			// 2. PlayerController를 통한 입력 컨텍스트 전환 (신규 로직)
			if (ABossRushPlayerController* PC = Cast<ABossRushPlayerController>(Character->GetController()))
			{
				PC->SetGimmickInputMode(bBlock);
			}
		}
	}
}
