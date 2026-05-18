// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BossRushPlayerController.h"
#include "EnhancedInputSubsystems.h"

void ABossRushPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 초기 설정: 기본 컨텍스트 추가
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (DefaultMappingContext)
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void ABossRushPlayerController::SetGimmickInputMode(bool bEnabled)
{
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (bEnabled)
		{
			// 기믹 모드: 기본 입력 제거, 기믹 입력 추가
			Subsystem->RemoveMappingContext(DefaultMappingContext);
			if (GimmickMappingContext)
			{
				Subsystem->AddMappingContext(GimmickMappingContext, 1);
			}
		}
		else
		{
			// 일반 모드: 기믹 입력 제거, 기본 입력 복구
			Subsystem->RemoveMappingContext(GimmickMappingContext);
			if (DefaultMappingContext)
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}
}
