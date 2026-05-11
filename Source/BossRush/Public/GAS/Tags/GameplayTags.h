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

	/* --- ����� ���� �±׵� ���� --- */
	FGameplayTag SprintStateTag;
	FGameplayTag DashStateTag;
	FGameplayTag SprintAttackTag;
	FGameplayTag DashAttackTag;
	FGameplayTag NormalAttackEventTag;
	FGameplayTag AimingStateTag;
	FGameplayTag NormalSkillEventTag;
	FGameplayTag UltraSkillEventTag;

	FGameplayTag RopeAimStateTag;
	FGameplayTag RopeActionStateTag;

private:
	static FGameplayTags GameplayTags;
};