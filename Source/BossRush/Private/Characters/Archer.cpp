// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Archer.h"
#include "Characters/Weapons/CArrow.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GAS/Tags/GameplayTags.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "ActionDatas.h"
#include "Net/UnrealNetwork.h"
#include "Abilities/GameplayAbilityTargetActor.h"

AArcher::AArcher()
{
}

void AArcher::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AArcher, CurrentArrow);
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

	// 화살 풀 초기화 (서버에서만 수행)
	if (HasAuthority() && ArrowClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		for (int32 i = 0; i < ArrowPoolSize; i++)
		{
			ACArrow* Arrow = GetWorld()->SpawnActor<ACArrow>(ArrowClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
			if (Arrow)
			{
				Arrow->Deactivate();
				ArrowPool.Add(Arrow);
			}
		}
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

	// 조준 상태가 아니면 현재 장착된 화살을 숨김
	if (CurrentArrow)
	{
		CurrentArrow->SetActorHiddenInGame(!bIsAiming);
	}
}

void AArcher::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	// Archer 전용 입력 바인딩이 필요하다면 여기에 추가 (현재는 부모 클래스에서 NormalAction 바인딩을 처리함)
}

void AArcher::StartAim()
{
	StopSprint();

	if (HasAuthority())
	{
		GetOrCreateTargetActor();
	}

	if (CurrentArrow)
	{
		CurrentArrow->SetActorHiddenInGame(false);
	}

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
	if (HasAuthority() && TargetActor)
	{
		TargetActor->Destroy();
		TargetActor = nullptr;
	}

	if (CurrentArrow)
	{
		CurrentArrow->SetActorHiddenInGame(true);
	}

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
	if (!AbilitySystemComponent)
	{
		return;
	}

	// 1. Sprint Attack (부모 로직 재활용)
	if (HandleSprintAttack()) return;

	// 2. Dash Attack (부모 로직 재활용)
	if (HandleDashAttack()) return;

	// 3. Aim Attack (Archer 전용 로직)
	if (AbilitySystemComponent->HasMatchingGameplayTag(FGameplayTags::Get().AimingStateTag))
	{
		if (AimAttackAbilityClass && !GetCurrentMontage())
		{
			AbilitySystemComponent->TryActivateAbilityByClass(AimAttackAbilityClass, true);
		}
		return;
	}

	// 4. Normal Attack (부모 로직 재활용)
	HandleNormalComboAttack();
}

void AArcher::DrawArrow()
{
	if (!HasAuthority()) return;

	// 이미 화살을 쥐고 있다면 중복 생성 방지
	if (CurrentArrow) return;

	CurrentArrow = GetArrowFromPool();
	if (CurrentArrow)
	{
		CurrentArrow->Prepare();
		
		CurrentArrow->SetActorHiddenInGame(false);

		FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, false);
		CurrentArrow->AttachToComponent(GetMesh(), AttachRules, ArrowSocketName);
	}
}

void AArcher::ShootArrow()
{
	if (!HasAuthority()) return;

	if (CurrentArrow)
	{
		CurrentArrow->SetActorHiddenInGame(false);

		FVector LaunchDirection = GetActorForwardVector();

		// 조준 중일 때 타겟 정보가 있으면 해당 방향으로 발사
		bool bIsAiming = AbilitySystemComponent && AbilitySystemComponent->HasMatchingGameplayTag(FGameplayTags::Get().AimingStateTag);
		
		if (bIsAiming)
		{
			// 타겟 위치가 유효한지(0,0,0이 아닌지) 확인
			if (bHasAimTarget && !AimTargetLocation.IsNearlyZero())
			{
				LaunchDirection = (AimTargetLocation - CurrentArrow->GetActorLocation()).GetSafeNormal();
			}
			else if (FollowCamera)
			{
				// 타겟 데이터가 없으면 카메라의 정면 방향을 사용
				LaunchDirection = FollowCamera->GetForwardVector();
			}
		}

		CurrentArrow->Launch(LaunchDirection);
		CurrentArrow = nullptr;
		ClearAimTarget();
	}
}

ACArrow* AArcher::GetArrowFromPool()
{
	for (ACArrow* Arrow : ArrowPool)
	{
		if (Arrow && !Arrow->IsInUse())
		{
			return Arrow;
		}
	}

	return nullptr;
}

AGameplayAbilityTargetActor* AArcher::GetOrCreateTargetActor()
{
	if (TargetActor) return TargetActor;

	if (TargetActorClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		TargetActor = GetWorld()->SpawnActor<AGameplayAbilityTargetActor>(TargetActorClass, SpawnParams);
	}

	return TargetActor;
}
