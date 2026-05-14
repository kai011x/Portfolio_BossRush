#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_Targeting.generated.h"

/**
 * 타겟팅 시스템을 활성화하고 관리하는 어빌리티입니다.
 */
UCLASS()
class BOSSRUSH_API UGA_Targeting : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_Targeting();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	UPROPERTY()
	class ACharacterBase* OwnerCharacter;

	UPROPERTY()
	class UTargetingComponent* TargetingComponent;
};
