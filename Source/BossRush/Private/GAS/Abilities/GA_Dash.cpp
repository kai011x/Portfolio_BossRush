// GA_Dash.cpp

#include "GAS/Abilities/GA_Dash.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_ApplyRootMotionConstantForce.h"
#include "AbilitySystemComponent.h"
#include "Characters/CharacterBase.h"


UGA_Dash::UGA_Dash()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

void UGA_Dash::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ACharacterBase* Character = Cast<ACharacterBase>(ActorInfo->AvatarActor.Get());

	DashMontageTable = Character->DT_Montages;

	if (!Character || !DashMontageTable)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	if (DashRegenEffectClass)
	{
		UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
		if (ASC)
		{
			// 이펙트 컨텍스트 생성 (누가 쐈는지 등 정보)
			FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
			ContextHandle.AddSourceObject(this);

	
			FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(DashRegenEffectClass, 1.0f, ContextHandle);

			if (SpecHandle.IsValid())
			{
				ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			}
		}
	}

	float DashStrength = Character->GetAttributeSet()->GetDashStrength();

	// 1. 월드 기준 입력 방향 가져오기
	FVector WorldInputDir = GetDashDirection(Character);

	// 2. 월드 벡터를 캐릭터 로컬 좌표로 변환 (Unrotate)
	// (예: 캐릭터가 북쪽을 보고 있을 때 동쪽 입력 -> 로컬에서는 '오른쪽'이 됨)
	FVector LocalDir = Character->GetActorRotation().UnrotateVector(WorldInputDir);

	// 3. 로컬 방향을 기반으로 태그 결정 (Front/Back/Left/Right)
	FGameplayTag TargetTag = DetermineDashTag(LocalDir);

	// 4. 태그와 일치하는 몽타주 데이터를 테이블에서 직접 검색 (RowName 안 씀)
	FMontageData* FoundData = FindMontageDataByTag(DashMontageTable, TargetTag);

	// 5. 몽타주 재생
	UE_LOG(LogTemp, Log, TEXT("Dash Direction: X=%f, Y=%f, Z=%f"), LocalDir.X, LocalDir.Y, LocalDir.Z);

	if (FoundData && FoundData->Montage)
	{
		UE_LOG(LogTemp, Log, TEXT("Playing Dash Montage for Tag: %s"), *TargetTag.ToString());

		UAbilityTask_ApplyRootMotionConstantForce* ForceTask = UAbilityTask_ApplyRootMotionConstantForce::ApplyRootMotionConstantForce(
			this,                               // Context (This Ability)
			FName("DashForce"),                 // Task Name (아무거나)
			WorldInputDir,                      // 밀어줄 방향 (World Space)
			DashStrength,                       // 힘의 크기
			0.2f,                               // 지속 시간 (초)
			false,                              // IsAdditive (기존 속도에 더할지)
			nullptr,                            // Strength Curve (커브 없으면 일정하게 밈)
			ERootMotionFinishVelocityMode::MaintainLastRootMotionVelocity, // 끝난 후 속도 처리
			FVector::ZeroVector,                // SetVelocity 모드일 때의 속도
			0.0f,                               // ClampVelocity 모드일 때의 제한 속도
			false                               // Gravity (중력 적용 여부)
		);


		UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this,
			FName("DashTask"),
			FoundData->Montage,
			FoundData->PlayRate,
			FName("DefaultSlot"),
			false
		);

		ForceTask->ReadyForActivation();



		// 태스크 종료 이벤트 연결
		Task->OnCompleted.AddDynamic(this, &UGA_Dash::OnMontageEnded);
		Task->OnBlendOut.AddDynamic(this, &UGA_Dash::OnMontageEnded);
		Task->OnInterrupted.AddDynamic(this, &UGA_Dash::OnMontageEnded);
		Task->OnCancelled.AddDynamic(this, &UGA_Dash::OnMontageEnded);

		Task->ReadyForActivation();
	}
	else
	{
		// 몽타주를 못 찾았으면 안전하게 종료
		UE_LOG(LogTemp, Warning, TEXT("Failed to find Montage for Tag: %s"), *TargetTag.ToString());
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}
}

// 방향 벡터 -> 태그 변환 로직
FGameplayTag UGA_Dash::DetermineDashTag(FVector LocalDirection)
{
	// 입력이 거의 없으면(0,0) 앞(Front)으로 간주
	if (LocalDirection.IsNearlyZero())
	{
		return FGameplayTag::RequestGameplayTag(FName("Character.State.Dash.Front"));
	}

	// X(앞뒤)와 Y(좌우) 중 절댓값이 큰 쪽이 주된 방향
	if (FMath::Abs(LocalDirection.X) >= FMath::Abs(LocalDirection.Y))
	{
		if (LocalDirection.X > 0)
			return FGameplayTag::RequestGameplayTag(FName("Character.State.Dash.Front"));
		else
			return FGameplayTag::RequestGameplayTag(FName("Character.State.Dash.Back"));
	}
	else
	{
		if (LocalDirection.Y > 0)
			return FGameplayTag::RequestGameplayTag(FName("Character.State.Dash.Right"));
		else
			return FGameplayTag::RequestGameplayTag(FName("Character.State.Dash.Left"));
	}
}

// 태그로 데이터 테이블 검색 로직 (핵심)
FMontageData* UGA_Dash::FindMontageDataByTag(UDataTable* Table, FGameplayTag SearchTag)
{
	if (!Table) return nullptr;

	// 1. 테이블의 모든 Row 이름들을 가져옵니다.
	TArray<FName> RowNames = Table->GetRowNames();

	// 2. 모든 Row를 하나씩 돌면서 검사합니다.
	for (const FName& RowName : RowNames)
	{
		// 데이터를 가져옴
		FMontageData* Data = Table->FindRow<FMontageData>(RowName, TEXT("FindMontageByTag"));

		// 3. 데이터 안에 있는 StateTag가 우리가 찾는 태그와 똑같은지 확인합니다.
		if (Data && Data->StateTag.MatchesTagExact(SearchTag))
		{
			return Data; // 찾았으면 리턴
		}
	}

	return nullptr; // 끝까지 못 찾음
}

FVector UGA_Dash::GetDashDirection(AActor* SourceActor)
{
	ACharacter* Character = Cast<ACharacter>(SourceActor);
	if (!Character) return SourceActor->GetActorForwardVector();

	FVector InputVector = Character->GetLastMovementInputVector();
	if (InputVector.IsNearlyZero()) return Character->GetActorForwardVector();

	return InputVector.GetSafeNormal();
}

void UGA_Dash::OnMontageEnded()
{
	bool bReplicateEndAbility = true;
	bool bWasCancelled = false;

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}
