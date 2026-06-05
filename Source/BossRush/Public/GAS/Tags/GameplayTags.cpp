#include "GameplayTags.h"
#include "GameplayTagsManager.h"

FGameplayTags FGameplayTags::GameplayTags;

void FGameplayTags::InitializeNativeGameplayTags()
{
	// �̱��� �ν��Ͻ��� �±׸� �����Ͽ� ��� (�̸�, �ּ�(����) ����)
	GameplayTags.DefenseStateTag = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Character.State.Defense"));

	GameplayTags.ChaseStateTag = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Character.State.Chase"));

	GameplayTags.GimmickStateTag = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Character.State.Gimmick"));

	GameplayTags.GroggyStateTag = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Character.State.Groggy"));

	GameplayTags.SprintStateTag = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Character.State.Sprint"));

	GameplayTags.DashStateTag = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Status.Dash.CanDashAttack"));

	GameplayTags.SprintAttackTag = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Event.Attack.Sprint"));

	GameplayTags.DashAttackTag = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Event.Attack.Dash"));

	GameplayTags.NormalAttackEventTag = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Event.Attack.NormalInput"));

	GameplayTags.AimingStateTag = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Character.State.Aiming"));

	GameplayTags.TargetingStateTag = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Character.State.Targeting"));

	GameplayTags.StatusSuperArmorTag = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Character.Status.SuperArmor"));

	GameplayTags.StatusUntouchableTag = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Character.Status.Untouchable"));

	GameplayTags.NormalSkillEventTag = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Event.Attack.NormalSkill"));

	GameplayTags.UltraSkillEventTag = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Event.Attack.UltraSkill"));

	GameplayTags.RopeAimStateTag = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Character.State.Rope.Aim"));

	GameplayTags.RopeActionStateTag = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Character.State.Rope.Action"));

	GameplayTags.DamageMultiplierDataTag = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Data.DamageMultiplier"));

	GameplayTags.HitTypeDataTag = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Data.HitType"));

	GameplayTags.HitIdxDataTag = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Data.HitIdx"));

	GameplayTags.LaunchDistanceDataTag = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Data.LaunchDistance"));

	GameplayTags.LaunchHeightDataTag = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Data.LaunchHeight"));

	GameplayTags.BlockInputEventTag = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Event.BlockInput"));

	GameplayTags.AbilitiesChangedEventTag = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Event.AbilitiesChanged"));

	GameplayTags.HittedStateTag = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Character.State.Hitted"));

	GameplayTags.StrongAttackEventTag = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Event.Attack.StrongInput"));

	GameplayTags.DashFrontStateTag = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Character.State.Dash.Front"));

	GameplayTags.DashBackStateTag = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Character.State.Dash.Back"));

	GameplayTags.DashRightStateTag = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Character.State.Dash.Right"));

	GameplayTags.DashLeftStateTag = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Character.State.Dash.Left"));

	GameplayTags.DashRegenStatusTag = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Status.Dash.Regen"));

	GameplayTags.CharacterStateAttackTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Character.State.Attack"));
	GameplayTags.CharacterStateCastingTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Character.State.Casting"));
	GameplayTags.CharacterStateDashTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Character.State.Dash"));
	GameplayTags.CharacterStateDeadTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Character.State.Dead"));
	GameplayTags.CharacterStateHitTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Character.State.Hit"));
	GameplayTags.CharacterStateIdleTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Character.State.Idle"));
	GameplayTags.CharacterStateJumpTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Character.State.Jump"));
	GameplayTags.CharacterStateRunTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Character.State.Run"));
	GameplayTags.CooldownDashTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Dash"));
	GameplayTags.DataDamageTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Data.Damage"));
	GameplayTags.EventAttackCheckTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Event.Attack.Check"));
	GameplayTags.EventAttackSubactionTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Event.Attack.SubAction"));
	GameplayTags.EventAttackSuccessTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Event.Attack.Success"));
	GameplayTags.EventAttackConditionAttackTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Event.Attack.ConditionAttack"));
	GameplayTags.EventHitscanEndTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Event.Hitscan.End"));
	GameplayTags.EventHitscanStartTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Event.Hitscan.Start"));
	GameplayTags.GameplayabilityAttackMeleeTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayAbility.Attack.Melee"));
	GameplayTags.GameplayabilityMeleeattackTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayAbility.MeleeAttack"));
	GameplayTags.GameplayabilityMeleeattackFighterTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayAbility.MeleeAttack.Fighter"));
	GameplayTags.GameplayabilityMovementDashTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayAbility.Movement.Dash"));
	GameplayTags.GameplaycueDamageBurstTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Damage.Burst"));
	GameplayTags.GameplaycueDashActiveTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Gameplaycue.Dash.Active"));
	GameplayTags.GameplaycueUltraSkillFighterBeamTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Character.UltraSkill.Fighter.Beam"));
	GameplayTags.GameplaycueCameraShakeTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.CameraShake"));
	GameplayTags.StatusStaminaRegenTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Status.Stamina.Regen"));
}