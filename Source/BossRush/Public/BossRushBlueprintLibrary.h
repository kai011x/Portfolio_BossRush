#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayTagContainer.h"
#include "ActionDatas.h"
#include "BossRushBlueprintLibrary.generated.h"

UCLASS()
class BOSSRUSH_API UBossRushBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * 소스 액터에서 타겟 액터로 데미지를 적용합니다. GAS 시스템을 기반으로 동작합니다.
	 * @param SourceActor 공격자
	 * @param TargetActor 피해자
	 * @param HitInfo 데미지 및 물리 효과 정보
	 */
	UFUNCTION(BlueprintCallable, Category = "BossRush|Gameplay")
	static void ApplyDamageToTarget(AActor* SourceActor, AActor* TargetActor, const FHitInfo& HitInfo);

	UFUNCTION(BlueprintPure, Category = "BossRush|Tags")
	static FGameplayTag GetSprintStateTag();

	UFUNCTION(BlueprintPure, Category = "BossRush|Tags")
	static FGameplayTag GetDashStateTag();

	UFUNCTION(BlueprintPure, Category = "BossRush|Tags")
	static FGameplayTag GetSprintAttackTag();

	UFUNCTION(BlueprintPure, Category = "BossRush|Tags")
	static FGameplayTag GetDashAttackTag();

	UFUNCTION(BlueprintPure, Category = "BossRush|Tags")
	static FGameplayTag GetNormalAttackEventTag();

	UFUNCTION(BlueprintPure, Category = "BossRush|Tags")
	static FGameplayTag GetAimingStateTag();

	UFUNCTION(BlueprintPure, Category = "BossRush|Tags")
	static FGameplayTag GetTargetingStateTag();

	UFUNCTION(BlueprintPure, Category = "BossRush|Tags")
	static FGameplayTag GetNormalSkillEventTag();

	UFUNCTION(BlueprintPure, Category = "BossRush|Tags")
	static FGameplayTag GetUltraSkillEventTag();

	UFUNCTION(BlueprintPure, Category = "BossRush|Tags")
	static FGameplayTag GetRopeAimStateTag();

	UFUNCTION(BlueprintPure, Category = "BossRush|Tags")
	static FGameplayTag GetRopeActionStateTag();

	UFUNCTION(BlueprintPure, Category = "BossRush|Tags")
	static FGameplayTag GetDamageMultiplierDataTag();

	UFUNCTION(BlueprintPure, Category = "BossRush|Tags")
	static FGameplayTag GetBlockInputEventTag();

	UFUNCTION(BlueprintPure, Category = "BossRush|Tags")
	static FGameplayTag GetAbilitiesChangedEventTag();

	UFUNCTION(BlueprintPure, Category = "BossRush|Tags")
	static FGameplayTag GetHittedStateTag();

	UFUNCTION(BlueprintPure, Category = "BossRush|Tags")
	static FGameplayTag GetStrongAttackEventTag();

	UFUNCTION(BlueprintPure, Category = "BossRush|Tags")
	static FGameplayTag GetDashFrontStateTag();

	UFUNCTION(BlueprintPure, Category = "BossRush|Tags")
	static FGameplayTag GetDashBackStateTag();

	UFUNCTION(BlueprintPure, Category = "BossRush|Tags")
	static FGameplayTag GetDashRightStateTag();

	UFUNCTION(BlueprintPure, Category = "BossRush|Tags")
	static FGameplayTag GetDashLeftStateTag();

	UFUNCTION(BlueprintPure, Category = "BossRush|Tags")
	static FGameplayTag GetDashRegenStatusTag();

	UFUNCTION(BlueprintPure, Category = "BossRush|Tags")
	static FGameplayTag GetCharacterStateAttackTag();

	UFUNCTION(BlueprintPure, Category = "BossRush|Tags")
	static FGameplayTag GetCharacterStateCastingTag();

	UFUNCTION(BlueprintPure, Category = "BossRush|Tags")
	static FGameplayTag GetCharacterStateDashTag();

	UFUNCTION(BlueprintPure, Category = "BossRush|Tags")
	static FGameplayTag GetCharacterStateDeadTag();

	UFUNCTION(BlueprintPure, Category = "BossRush|Tags")
	static FGameplayTag GetCharacterStateHitTag();

	UFUNCTION(BlueprintPure, Category = "BossRush|Tags")
	static FGameplayTag GetCharacterStateIdleTag();

	UFUNCTION(BlueprintPure, Category = "BossRush|Tags")
	static FGameplayTag GetCharacterStateJumpTag();

	UFUNCTION(BlueprintPure, Category = "BossRush|Tags")
	static FGameplayTag GetCharacterStateRunTag();

	UFUNCTION(BlueprintPure, Category = "BossRush|Tags")
	static FGameplayTag GetCooldownDashTag();

	UFUNCTION(BlueprintPure, Category = "BossRush|Tags")
	static FGameplayTag GetDataDamageTag();

	UFUNCTION(BlueprintPure, Category = "BossRush|Tags")
	static FGameplayTag GetEventAttackCheckTag();

	UFUNCTION(BlueprintPure, Category = "BossRush|Tags")
	static FGameplayTag GetEventAttackSubactionTag();

	UFUNCTION(BlueprintPure, Category = "BossRush|Tags")
	static FGameplayTag GetEventHitscanEndTag();

	UFUNCTION(BlueprintPure, Category = "BossRush|Tags")
	static FGameplayTag GetEventHitscanStartTag();

	UFUNCTION(BlueprintPure, Category = "BossRush|Tags")
	static FGameplayTag GetGameplayabilityAttackMeleeTag();

	UFUNCTION(BlueprintPure, Category = "BossRush|Tags")
	static FGameplayTag GetGameplayabilityMeleeattackTag();

	UFUNCTION(BlueprintPure, Category = "BossRush|Tags")
	static FGameplayTag GetGameplayabilityMeleeattackFighterTag();

	UFUNCTION(BlueprintPure, Category = "BossRush|Tags")
	static FGameplayTag GetGameplayabilityMovementDashTag();

	UFUNCTION(BlueprintPure, Category = "BossRush|Tags")
	static FGameplayTag GetGameplaycueDamageBurstTag();

	UFUNCTION(BlueprintPure, Category = "BossRush|Tags")
	static FGameplayTag GetGameplaycueDashActiveTag();

	UFUNCTION(BlueprintPure, Category = "BossRush|Tags")
	static FGameplayTag GetStatusStaminaRegenTag();

};
