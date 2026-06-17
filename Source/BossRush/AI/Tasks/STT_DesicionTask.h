// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StateTreeTaskBase.h"
#include "STT_DesicionTask.generated.h"

/**
 * STT_DesicionTask가 사용하는 인스턴스 데이터 구조체
 */
USTRUCT(BlueprintType)
struct BOSSRUSH_API FSTT_DesicionTaskInstanceData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Context")
	TObjectPtr<class ABossBase> BossBase = nullptr;
};

/**
 * Idle 상태에서 보스의 다음 행동 패턴 결정을 내리는 StateTree Task
 */
USTRUCT(BlueprintType, meta = (Category = "Common", DisplayName = "STT_DesicionTask"))
struct BOSSRUSH_API FSTT_DesicionTask : public FStateTreeTaskBase
{
	GENERATED_BODY()

	typedef FSTT_DesicionTaskInstanceData InstanceDataType;

	FSTT_DesicionTask() = default;

	virtual const UScriptStruct* GetInstanceDataType() const override { return InstanceDataType::StaticStruct(); }
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
};
