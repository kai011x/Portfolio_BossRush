// GA_Jump.cpp

#include "GAS/Abilities/GA_Jump.h"
#include "Abilities/Tasks/AbilityTask_WaitMovementModeChange.h"
#include "Characters/CharacterBase.h"


void UGA_Jump::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ACharacterBase* Character = Cast<ACharacterBase>(ActorInfo->AvatarActor.Get());

	UE_LOG(LogTemp, Warning, TEXT("Activate Jump Ability"));

	if (Character)
	{

		Character->Jump();

		UE_LOG(LogTemp, Warning, TEXT("Jump Ability Activated"));

		UAbilityTask_WaitMovementModeChange* Task = UAbilityTask_WaitMovementModeChange::CreateWaitMovementModeChange(
			this,
			MOVE_Walking
		);

		// 3. 델리게이트 연결
		// 인자가 맞는 함수를 연결해야 에러가 안 납니다.
		Task->OnChange.AddDynamic(this, &UGA_Jump::OnLanded);

		// 4. 태스크 시작
		Task->ReadyForActivation();
	}
}

// [수정됨] 인자로 EMovementMode NewMode를 받아야 합니다!
void UGA_Jump::OnLanded(EMovementMode NewMode)
{
	// 착지했으니 어빌리티 종료
	bool bReplicateEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}