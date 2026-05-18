#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * ���� �� ��� ����Ƽ�� �����÷��� �±׸� �����ϴ� ���� �̱��� ����ü�Դϴ�.
 */
struct FGameplayTags
{
public:
	// �̱��� �ν��Ͻ��� �������� ���� �Լ�
	static const FGameplayTags& Get() { return GameplayTags; }

	// �±׵��� ������ ����ϴ� �Լ�
	static void InitializeNativeGameplayTags();

	/* --- Character States (캐릭터 상태) --- */
	FGameplayTag SprintStateTag;       // 캐릭터 질주 상태
	FGameplayTag DashStateTag;         // 대시 가능 상태 (공격 연계용)
	FGameplayTag AimingStateTag;       // 조준 상태
	FGameplayTag TargetingStateTag;    // 타겟팅 활성화 상태
	FGameplayTag HittedStateTag;       // 피격 리액션 중 상태
	FGameplayTag RopeAimStateTag;      // 로프 조준 상태
	FGameplayTag RopeActionStateTag;   // 로프 액션(이동) 중 상태

	FGameplayTag DashFrontStateTag;    // 전방 대시 상태
	FGameplayTag DashBackStateTag;     // 후방 대시 상태
	FGameplayTag DashRightStateTag;    // 우측 대시 상태
	FGameplayTag DashLeftStateTag;     // 좌측 대시 상태

	FGameplayTag CharacterStateAttackTag;
	FGameplayTag CharacterStateCastingTag;
	FGameplayTag CharacterStateDashTag;
	FGameplayTag CharacterStateDeadTag;
	FGameplayTag CharacterStateHitTag;
	FGameplayTag CharacterStateIdleTag;
	FGameplayTag CharacterStateJumpTag;
	FGameplayTag CharacterStateRunTag;

	/* --- Events (이벤트 및 입력) --- */
	FGameplayTag NormalAttackEventTag; // 일반 공격 입력 이벤트
	FGameplayTag StrongAttackEventTag; // 강공격 입력 이벤트
	FGameplayTag NormalSkillEventTag;  // 일반 스킬 이벤트
	FGameplayTag UltraSkillEventTag;   // 궁극기 이벤트
	FGameplayTag SprintAttackTag;      // 질주 공격 이벤트
	FGameplayTag DashAttackTag;        // 대시 공격 이벤트
	FGameplayTag BlockInputEventTag;   // 입력 차단 이벤트
	FGameplayTag AbilitiesChangedEventTag; // 어빌리티 변경 알림 이벤트

	FGameplayTag EventAttackCheckTag;
	FGameplayTag EventAttackSubactionTag;
	FGameplayTag EventAttackSuccessTag;
	FGameplayTag EventAttackConditionAttackTag;
	FGameplayTag EventHitscanEndTag;
	FGameplayTag EventHitscanStartTag;

	/* --- Data & Parameters (데이터 전달용) --- */
	FGameplayTag DamageMultiplierDataTag;
	FGameplayTag HitTypeDataTag;
	FGameplayTag HitIdxDataTag;
	FGameplayTag LaunchDistanceDataTag;
	FGameplayTag LaunchHeightDataTag; // 데미지 배율 전달 태그 (SetByCaller)
	FGameplayTag DataDamageTag;           // 데미지 데이터 태그

	/* --- Status & Cooldowns (상태 효과 및 쿨다운) --- */
	FGameplayTag DashRegenStatusTag;   // 대시 게이지 재생 중 상태
	FGameplayTag StatusStaminaRegenTag; // 스태미나 재생 중 상태
	FGameplayTag StatusSuperArmorTag;   // 슈퍼아머 상태
	FGameplayTag StatusUntouchableTag;  // 무적 상태
	FGameplayTag CooldownDashTag;      // 대시 쿨다운 태그

	/* --- Gameplay Abilities (어빌리티 관련) --- */
	FGameplayTag GameplayabilityAttackMeleeTag;
	FGameplayTag GameplayabilityMeleeattackTag;
	FGameplayTag GameplayabilityMeleeattackFighterTag;
	FGameplayTag GameplayabilityMovementDashTag;

	/* --- Gameplay Cues (이펙트 및 사운드) --- */
	FGameplayTag GameplaycueDamageBurstTag;
	FGameplayTag GameplaycueDashActiveTag;
	FGameplayTag GameplaycueUltraSkillFighterBeamTag;
	FGameplayTag GameplaycueCameraShakeTag;

	private:
	static FGameplayTags GameplayTags;
};