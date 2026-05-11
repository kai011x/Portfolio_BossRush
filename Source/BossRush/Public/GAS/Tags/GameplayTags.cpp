#include "GameplayTags.h"
#include "GameplayTagsManager.h"

FGameplayTags FGameplayTags::GameplayTags;

void FGameplayTags::InitializeNativeGameplayTags()
{
	// �̱��� �ν��Ͻ��� �±׸� �����Ͽ� ��� (�̸�, �ּ�(����) ����)
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

	GameplayTags.NormalSkillEventTag = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Event.Attack.NormalSkill"));

	GameplayTags.UltraSkillEventTag = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Event.Attack.UltraSkill"));

	GameplayTags.RopeAimStateTag = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Character.State.Rope.Aim"));

	GameplayTags.RopeActionStateTag = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Character.State.Rope.Action"));
}