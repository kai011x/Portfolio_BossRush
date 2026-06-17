#include "GAS/Abilities/GA_Targeting.h"
#include "Characters/CharacterBase.h"
#include "Components/TargetingComponent.h"
#include "GAS/Tags/GameplayTags.h"

UGA_Targeting::UGA_Targeting()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	
	// CancelAbilities 대응을 위해 AbilityTags에 태그 설정
	SetAssetTags(FGameplayTagContainer(FGameplayTags::Get().TargetingStateTag));

	// 실행 시 캐릭터에게 부여할 태그
	ActivationOwnedTags.AddTag(FGameplayTags::Get().TargetingStateTag);
}

void UGA_Targeting::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	OwnerCharacter = Cast<ACharacterBase>(ActorInfo->AvatarActor.Get());
	if (OwnerCharacter)
	{
		TargetingComponent = OwnerCharacter->GetTargetingComponent();
		if (TargetingComponent)
		{
			TargetingComponent->StartTargeting();
			
			// 타겟을 찾았다면 캐릭터 설정 업데이트
			if (TargetingComponent->IsTargeting())
			{
				OwnerCharacter->SetTargetingMode(true);
			}
			else
			{
				EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
				return;
			}
		}
	}
}

void UGA_Targeting::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (TargetingComponent)
	{
		TargetingComponent->EndTargeting();
	}

	if (OwnerCharacter)
	{
		OwnerCharacter->SetTargetingMode(false);
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
