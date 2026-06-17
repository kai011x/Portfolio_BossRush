// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Tasks/STT_DesicionTask.h"
#include "Characters/Bosses/BossBase.h"
#include "StateTreeExecutionContext.h"

EStateTreeRunStatus FSTT_DesicionTask::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	InstanceDataType& InstanceData = Context.GetInstanceData<InstanceDataType>(*this);

	InstanceData.BossBase = Cast<ABossBase>(Context.GetOwner());

	if (InstanceData.BossBase)
	{
		return EStateTreeRunStatus::Succeeded;
	}

	return EStateTreeRunStatus::Failed;
}
