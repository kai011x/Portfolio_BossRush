// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Archer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "GAS/Tags/GameplayTags.h"

AArcher::AArcher()
{
}

void AArcher::BeginPlay()
{
	Super::BeginPlay();

	if (CameraBoom)
	{
		DefaultArmLength = CameraBoom->TargetArmLength;
		DefaultSocketOffset = CameraBoom->SocketOffset;
	}

	if (FollowCamera)
	{
		DefaultFOV = FollowCamera->FieldOfView;
	}
}

void AArcher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// AbilitySystemComponent에서 AimingStateTag 존재 여부 확인
	bool bIsAiming = AbilitySystemComponent && AbilitySystemComponent->HasMatchingGameplayTag(FGameplayTags::Get().AimingStateTag);

	float TargetLength = bIsAiming ? AimArmLength : DefaultArmLength;
	float TargetFOV = bIsAiming ? AimFOV : DefaultFOV;
	FVector TargetOffset = bIsAiming ? AimSocketOffset : DefaultSocketOffset;

	if (CameraBoom)
	{
		CameraBoom->TargetArmLength = FMath::FInterpTo(CameraBoom->TargetArmLength, TargetLength, DeltaTime, AimInterpSpeed);
		CameraBoom->SocketOffset = FMath::VInterpTo(CameraBoom->SocketOffset, TargetOffset, DeltaTime, AimInterpSpeed);
	}

	if (FollowCamera)
	{
		FollowCamera->FieldOfView = FMath::FInterpTo(FollowCamera->FieldOfView, TargetFOV, DeltaTime, AimInterpSpeed);
	}
}

void AArcher::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	//Normal Attack
	
	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (NormalAction) {
			EIC->BindAction(NormalAction, ETriggerEvent::Started, this, &ACharacterBase::OnNormalAttackInput);
		}
	}
}

void AArcher::StartAim()
{
	if (AbilitySystemComponent)
	{
		// 태그가 없을 때만 추가하여 중복 방지
		if (!AbilitySystemComponent->HasMatchingGameplayTag(FGameplayTags::Get().AimingStateTag))
		{
			AbilitySystemComponent->AddLooseGameplayTag(FGameplayTags::Get().AimingStateTag);
		}
	}
	bUseControllerRotationYaw = true;

	// Pitch 각도 제한 설정
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (PC->PlayerCameraManager)
		{
			PC->PlayerCameraManager->ViewPitchMin = -70.f;
			PC->PlayerCameraManager->ViewPitchMax = 70.f;

			// 현재 각도가 범위를 벗어나 있다면 즉시 보정
			FRotator ControlRot = PC->GetControlRotation();
			if (ControlRot.Pitch > 180.f) ControlRot.Pitch -= 360.f; // -180~180 범위로 정규화
			ControlRot.Pitch = FMath::Clamp(ControlRot.Pitch, -70.f, 70.f);
			PC->SetControlRotation(ControlRot);
		}
	}
}

void AArcher::StopAim()
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->RemoveLooseGameplayTag(FGameplayTags::Get().AimingStateTag);
	}
	bUseControllerRotationYaw = false;

	// Pitch 각도 제한 해제 (기본값으로 복구)
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (PC->PlayerCameraManager)
		{
			PC->PlayerCameraManager->ViewPitchMin = -89.9f;
			PC->PlayerCameraManager->ViewPitchMax = 89.9f;
		}
	}
}

void AArcher::OnNormalAttackInput()
{
	if (AbilitySystemComponent && AbilitySystemComponent->HasMatchingGameplayTag(FGameplayTags::Get().AimingStateTag))
	{

	}

	Super::OnNormalAttackInput();
}
