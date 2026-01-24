// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "NativeGameplayTags.h"
#include "GA_ComboAttack.generated.h"

/**
 * 
 */
 // 태그 선언
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Event_Attack_Input); // 입력 감지용
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Event_Attack_Check);


UCLASS()
class BOSSRUSH_API UGA_ComboAttack : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UGA_ComboAttack();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
public:
	void PlayComboAction();
	
	// 입력이 들어왔을 때 호출
	UFUNCTION()
	void OnInputReceived(FGameplayEventData Payload);

	// 몽타주에서 "CheckCombo" 이벤트를 보냈을 때 호출
	UFUNCTION()
	void OnComboCheckEvent(FGameplayEventData Payload);

	// 몽타주가 끝났을 때 (콤보 실패 혹은 종료)
	UFUNCTION()
	void OnMontageCompleted();


public:
	// 현재 콤보 인덱스
	int32 CurrentComboIndex;

	// 다음 공격 입력이 들어왔는지 체크하는 플래그
	bool bNextInputPressed;

	// 콤보 체크용 태그 (예: Event.Montage.CheckCombo)
	UPROPERTY(EditDefaultsOnly, Category = "Combo")
	FGameplayTag ComboCheckTag;

	UPROPERTY(EditDefaultsOnly, Category = "Combo")
	UDataTable* ComboDataTable;

	UPROPERTY()
	class UAbilityTask_WaitGameplayEvent* InputWaitTask;

	UPROPERTY()
	class UAbilityTask_PlayMontageAndWait* CurrentMontageTask;
};
