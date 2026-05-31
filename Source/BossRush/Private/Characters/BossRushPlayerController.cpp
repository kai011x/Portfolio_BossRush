// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BossRushPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Characters/CharacterBase.h"

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

void ABossRushPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Moving
		if (MoveAction)
		{
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABossRushPlayerController::Move);
		}

		// Looking
		if (LookAction)
		{
			EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABossRushPlayerController::Look);
		}

		// Jumping
		if (JumpAction)
		{
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ABossRushPlayerController::JumpTriggered);
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ABossRushPlayerController::JumpCompleted);
		}

		// Dash
		if (DashAction)
		{
			EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Started, this, &ABossRushPlayerController::DashTriggered);
		}

		// Normal Attack
		if (NormalAction)
		{
			EnhancedInputComponent->BindAction(NormalAction, ETriggerEvent::Started, this, &ABossRushPlayerController::NormalAttackTriggered);
		}

		// Sprint
		if (SprintAction)
		{
			EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ABossRushPlayerController::SprintStarted);
			EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ABossRushPlayerController::SprintCompleted);
			EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Canceled, this, &ABossRushPlayerController::SprintCompleted);
		}

		// Targeting
		if (TargetingAction)
		{
			EnhancedInputComponent->BindAction(TargetingAction, ETriggerEvent::Started, this, &ABossRushPlayerController::TargetingTriggered);
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

void ABossRushPlayerController::Move(const FInputActionValue& Value)
{
	if (ACharacterBase* CharacterBase = Cast<ACharacterBase>(GetPawn()))
	{
		CharacterBase->Move(Value);
	}
}

void ABossRushPlayerController::Look(const FInputActionValue& Value)
{
	if (ACharacterBase* CharacterBase = Cast<ACharacterBase>(GetPawn()))
	{
		CharacterBase->Look(Value);
	}
}

void ABossRushPlayerController::JumpTriggered(const FInputActionValue& Value)
{
	if (ACharacterBase* CharacterBase = Cast<ACharacterBase>(GetPawn()))
	{
		CharacterBase->BaseJump(Value);
	}
}

void ABossRushPlayerController::JumpCompleted(const FInputActionValue& Value)
{
	if (ACharacterBase* CharacterBase = Cast<ACharacterBase>(GetPawn()))
	{
		CharacterBase->StopJumping();
	}
}

void ABossRushPlayerController::DashTriggered(const FInputActionValue& Value)
{
	if (ACharacterBase* CharacterBase = Cast<ACharacterBase>(GetPawn()))
	{
		CharacterBase->Dash(Value);
	}
}

void ABossRushPlayerController::NormalAttackTriggered()
{
	if (ACharacterBase* CharacterBase = Cast<ACharacterBase>(GetPawn()))
	{
		CharacterBase->OnNormalAttackInput();
	}
}

void ABossRushPlayerController::SprintStarted()
{
	if (ACharacterBase* CharacterBase = Cast<ACharacterBase>(GetPawn()))
	{
		CharacterBase->StartSprint();
	}
}

void ABossRushPlayerController::SprintCompleted()
{
	if (ACharacterBase* CharacterBase = Cast<ACharacterBase>(GetPawn()))
	{
		CharacterBase->StopSprint();
	}
}

void ABossRushPlayerController::TargetingTriggered()
{
	if (ACharacterBase* CharacterBase = Cast<ACharacterBase>(GetPawn()))
	{
		CharacterBase->OnTargetingAction();
	}
}
