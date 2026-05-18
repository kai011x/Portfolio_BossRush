#include "BossRushBlueprintLibrary.h"
#include "GAS/Tags/GameplayTags.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Characters/CharacterBase.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "GameplayEffect.h"

void UBossRushBlueprintLibrary::ApplyDamageToTarget(AActor* SourceActor, AActor* TargetActor, const FHitInfo& HitInfo)
{
	if (!SourceActor || !TargetActor) return;

	IAbilitySystemInterface* SourceInterface = Cast<IAbilitySystemInterface>(SourceActor);
	IAbilitySystemInterface* TargetInterface = Cast<IAbilitySystemInterface>(TargetActor);

	if (SourceInterface && TargetInterface)
	{
		UAbilitySystemComponent* SourceASC = SourceInterface->GetAbilitySystemComponent();
		UAbilitySystemComponent* TargetASC = TargetInterface->GetAbilitySystemComponent();

		if (!SourceASC || !TargetASC) return;

		const FGameplayTags& GTags = FGameplayTags::Get();

		// 1. 무적 체크: 타겟이 무적 상태라면 데미지 로직 전체 무시
		if (TargetASC->HasMatchingGameplayTag(GTags.StatusUntouchableTag))
		{
			return;
		}

		// 데미지 이펙트 클래스 찾기 (공격자 기준)
		TSubclassOf<UGameplayEffect> EffectClass = nullptr;
		if (ACharacterBase* SourceChar = Cast<ACharacterBase>(SourceActor))
		{
			EffectClass = SourceChar->GetDamageEffectClass();
		}

		if (EffectClass)
		{
			FGameplayEffectContextHandle ContextHandle = SourceASC->MakeEffectContext();
			ContextHandle.AddInstigator(SourceActor, SourceActor);

			FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(EffectClass, 1.0f, ContextHandle);
			if (SpecHandle.IsValid())
			{
				// 정보들 주문서에 꼼꼼히 적기
				SpecHandle.Data.Get()->SetSetByCallerMagnitude(GTags.DamageMultiplierDataTag, HitInfo.DamageMultiplier);
				SpecHandle.Data.Get()->SetSetByCallerMagnitude(GTags.HitTypeDataTag, (float)HitInfo.HitType);
				SpecHandle.Data.Get()->SetSetByCallerMagnitude(GTags.HitIdxDataTag, (float)HitInfo.HitIdx);
				SpecHandle.Data.Get()->SetSetByCallerMagnitude(GTags.LaunchDistanceDataTag, HitInfo.LaunchDistance);
				SpecHandle.Data.Get()->SetSetByCallerMagnitude(GTags.LaunchHeightDataTag, HitInfo.LaunchHeight);

				// 2. 데미지 배달!
				SourceASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetASC);

				// 3. 리액션 처리 (슈퍼아머가 아닐 때만 실행)
				if (!TargetASC->HasMatchingGameplayTag(GTags.StatusSuperArmorTag))
				{
					if (ACharacterBase* TargetChar = Cast<ACharacterBase>(TargetActor))
					{
						TargetChar->PlayHitReact(HitInfo.HitType, HitInfo.HitIdx, HitInfo.LaunchDistance, HitInfo.LaunchHeight, SourceActor);
					}
				}

				// 성공 이벤트도 자동으로 쏴주기
				FGameplayEventData Payload;
				Payload.EventTag = GTags.EventAttackSuccessTag;
				Payload.Instigator = SourceActor;
				Payload.Target = TargetActor;
				Payload.ContextHandle = ContextHandle;
				SourceASC->HandleGameplayEvent(Payload.EventTag, &Payload);
			}
		}
	}
}

FGameplayTag UBossRushBlueprintLibrary::GetSprintStateTag()
{
	return FGameplayTags::Get().SprintStateTag;
}

FGameplayTag UBossRushBlueprintLibrary::GetDashStateTag()
{
	return FGameplayTags::Get().DashStateTag;
}

FGameplayTag UBossRushBlueprintLibrary::GetSprintAttackTag()
{
	return FGameplayTags::Get().SprintAttackTag;
}

FGameplayTag UBossRushBlueprintLibrary::GetDashAttackTag()
{
	return FGameplayTags::Get().DashAttackTag;
}

FGameplayTag UBossRushBlueprintLibrary::GetNormalAttackEventTag()
{
	return FGameplayTags::Get().NormalAttackEventTag;
}

FGameplayTag UBossRushBlueprintLibrary::GetAimingStateTag()
{
	return FGameplayTags::Get().AimingStateTag;
}

FGameplayTag UBossRushBlueprintLibrary::GetTargetingStateTag()
{
	return FGameplayTags::Get().TargetingStateTag;
}

FGameplayTag UBossRushBlueprintLibrary::GetNormalSkillEventTag()
{
	return FGameplayTags::Get().NormalSkillEventTag;
}

FGameplayTag UBossRushBlueprintLibrary::GetUltraSkillEventTag()
{
	return FGameplayTags::Get().UltraSkillEventTag;
}

FGameplayTag UBossRushBlueprintLibrary::GetRopeAimStateTag()
{
	return FGameplayTags::Get().RopeAimStateTag;
}

FGameplayTag UBossRushBlueprintLibrary::GetRopeActionStateTag()
{
	return FGameplayTags::Get().RopeActionStateTag;
}

FGameplayTag UBossRushBlueprintLibrary::GetDamageMultiplierDataTag()
{
	return FGameplayTags::Get().DamageMultiplierDataTag;
}

FGameplayTag UBossRushBlueprintLibrary::GetBlockInputEventTag()
{
	return FGameplayTags::Get().BlockInputEventTag;
}

FGameplayTag UBossRushBlueprintLibrary::GetAbilitiesChangedEventTag()
{
	return FGameplayTags::Get().AbilitiesChangedEventTag;
}

FGameplayTag UBossRushBlueprintLibrary::GetHittedStateTag()
{
	return FGameplayTags::Get().HittedStateTag;
}

FGameplayTag UBossRushBlueprintLibrary::GetStrongAttackEventTag()
{
	return FGameplayTags::Get().StrongAttackEventTag;
}

FGameplayTag UBossRushBlueprintLibrary::GetDashFrontStateTag()
{
	return FGameplayTags::Get().DashFrontStateTag;
}

FGameplayTag UBossRushBlueprintLibrary::GetDashBackStateTag()
{
	return FGameplayTags::Get().DashBackStateTag;
}

FGameplayTag UBossRushBlueprintLibrary::GetDashRightStateTag()
{
	return FGameplayTags::Get().DashRightStateTag;
}

FGameplayTag UBossRushBlueprintLibrary::GetDashLeftStateTag()
{
	return FGameplayTags::Get().DashLeftStateTag;
}

FGameplayTag UBossRushBlueprintLibrary::GetDashRegenStatusTag()
{
	return FGameplayTags::Get().DashRegenStatusTag;
}

FGameplayTag UBossRushBlueprintLibrary::GetCharacterStateAttackTag()
{
	return FGameplayTags::Get().CharacterStateAttackTag;
}

FGameplayTag UBossRushBlueprintLibrary::GetCharacterStateCastingTag()
{
	return FGameplayTags::Get().CharacterStateCastingTag;
}

FGameplayTag UBossRushBlueprintLibrary::GetCharacterStateDashTag()
{
	return FGameplayTags::Get().CharacterStateDashTag;
}

FGameplayTag UBossRushBlueprintLibrary::GetCharacterStateDeadTag()
{
	return FGameplayTags::Get().CharacterStateDeadTag;
}

FGameplayTag UBossRushBlueprintLibrary::GetCharacterStateHitTag()
{
	return FGameplayTags::Get().CharacterStateHitTag;
}

FGameplayTag UBossRushBlueprintLibrary::GetCharacterStateIdleTag()
{
	return FGameplayTags::Get().CharacterStateIdleTag;
}

FGameplayTag UBossRushBlueprintLibrary::GetCharacterStateJumpTag()
{
	return FGameplayTags::Get().CharacterStateJumpTag;
}

FGameplayTag UBossRushBlueprintLibrary::GetCharacterStateRunTag()
{
	return FGameplayTags::Get().CharacterStateRunTag;
}

FGameplayTag UBossRushBlueprintLibrary::GetCooldownDashTag()
{
	return FGameplayTags::Get().CooldownDashTag;
}

FGameplayTag UBossRushBlueprintLibrary::GetDataDamageTag()
{
	return FGameplayTags::Get().DataDamageTag;
}

FGameplayTag UBossRushBlueprintLibrary::GetEventAttackCheckTag()
{
	return FGameplayTags::Get().EventAttackCheckTag;
}

FGameplayTag UBossRushBlueprintLibrary::GetEventAttackSubactionTag()
{
	return FGameplayTags::Get().EventAttackSubactionTag;
}

FGameplayTag UBossRushBlueprintLibrary::GetEventHitscanEndTag()
{
	return FGameplayTags::Get().EventHitscanEndTag;
}

FGameplayTag UBossRushBlueprintLibrary::GetEventHitscanStartTag()
{
	return FGameplayTags::Get().EventHitscanStartTag;
}

FGameplayTag UBossRushBlueprintLibrary::GetGameplayabilityAttackMeleeTag()
{
	return FGameplayTags::Get().GameplayabilityAttackMeleeTag;
}

FGameplayTag UBossRushBlueprintLibrary::GetGameplayabilityMeleeattackTag()
{
	return FGameplayTags::Get().GameplayabilityMeleeattackTag;
}

FGameplayTag UBossRushBlueprintLibrary::GetGameplayabilityMeleeattackFighterTag()
{
	return FGameplayTags::Get().GameplayabilityMeleeattackFighterTag;
}

FGameplayTag UBossRushBlueprintLibrary::GetGameplayabilityMovementDashTag()
{
	return FGameplayTags::Get().GameplayabilityMovementDashTag;
}

FGameplayTag UBossRushBlueprintLibrary::GetGameplaycueDamageBurstTag()
{
	return FGameplayTags::Get().GameplaycueDamageBurstTag;
}

FGameplayTag UBossRushBlueprintLibrary::GetGameplaycueDashActiveTag()
{
	return FGameplayTags::Get().GameplaycueDashActiveTag;
}

FGameplayTag UBossRushBlueprintLibrary::GetStatusStaminaRegenTag()
{
	return FGameplayTags::Get().StatusStaminaRegenTag;
}

