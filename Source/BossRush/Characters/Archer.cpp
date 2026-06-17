// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Archer.h"
#include "Characters/Weapons/CArrow.h"
#include "Characters/Weapons/ACRope.h"
#include "Components/RopeActionComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GAS/Tags/GameplayTags.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "ActionDatas.h"
#include "Net/UnrealNetwork.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "Blueprint/UserWidget.h"

AArcher::AArcher()
{
	RopeActionComponent = CreateDefaultSubobject<URopeActionComponent>(TEXT("RopeActionComponent"));

	RightFootSphere = CreateDefaultSubobject<USphereComponent>(TEXT("RightFootSphere"));
	RightFootSphere->SetupAttachment(GetMesh(), TEXT("foot_r"));
	RightFootSphere->SetSphereRadius(10.0f);
	RightFootSphere->SetCollisionProfileName(TEXT("NoCollision"));

	CollisionShapes.Add(RightFootSphere);
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

	UpdateCameraZoom(DeltaTime);

	// AbilitySystemComponent에서 AimingStateTag 존재 여부 확인
	bool bIsAiming = AbilitySystemComponent && AbilitySystemComponent->HasMatchingGameplayTag(FGameplayTags::Get().AimingStateTag);

	// 조준 상태가 아니면 현재 장착된 화살을 숨김
	if (CurrentArrow)
	{
		CurrentArrow->SetActorHiddenInGame(!bIsAiming);
	}
}

void AArcher::UpdateCameraZoom(float DeltaTime)
{
	if (!AbilitySystemComponent) return;

	// 1. 조준 중(일반/로프)인지 확인
	bool bIsAiming = AbilitySystemComponent->HasMatchingGameplayTag(FGameplayTags::Get().AimingStateTag) ||
		AbilitySystemComponent->HasMatchingGameplayTag(FGameplayTags::Get().RopeAimStateTag);

	// 2. 현재 로프 이동 중(Action)인지 확인
	bool bIsRopeMoving = AbilitySystemComponent->HasMatchingGameplayTag(FGameplayTags::Get().RopeActionStateTag);

	// 로프 이동 중에는 조준 중이더라도 카메라를 축소함 (시야 확보)
	bool bShouldZoom = bIsAiming && !bIsRopeMoving;

	float TargetLength = bShouldZoom ? AimArmLength : DefaultArmLength;
	float TargetFOV = bShouldZoom ? AimFOV : DefaultFOV;
	FVector TargetOffset = bShouldZoom ? AimSocketOffset : DefaultSocketOffset;

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

void AArcher::StartAim() 
{
	if (!AbilitySystemComponent) return;

	// [추가] 로프 조준 중이면 일반 조준 불가
	if (AbilitySystemComponent->HasMatchingGameplayTag(FGameplayTags::Get().RopeAimStateTag))
	{
		return;
	}

	StopSprint();

	if (HasAuthority())
	{
		GetOrCreateTargetActor();
	}

	if (CurrentArrow)
	{
		CurrentArrow->SetActorHiddenInGame(false);
	}

	// 일반 조준 위젯 표시/
	if (IsLocallyControlled() && AimWidgetClass && !CurrentAimWidget)
	{
		CurrentAimWidget = CreateWidget<UUserWidget>(GetWorld(), AimWidgetClass);
		if (CurrentAimWidget)
		{
			CurrentAimWidget->AddToViewport();
		}
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

	// 일반 조준 위젯 제거
	if (CurrentAimWidget)
	{
		CurrentAimWidget->RemoveFromParent();
		CurrentAimWidget = nullptr;
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

void AArcher::SetRopeAimWidgetVisible(bool bVisible)
{
	if (!IsLocallyControlled()) return;

	if (bVisible)
	{
		if (RopeAimWidgetClass && !CurrentRopeAimWidget)
		{
			CurrentRopeAimWidget = CreateWidget<UUserWidget>(GetWorld(), RopeAimWidgetClass);
			if (CurrentRopeAimWidget)
			{
				CurrentRopeAimWidget->AddToViewport();
			}
		}
	}
	else
	{
		if (CurrentRopeAimWidget)
		{
			CurrentRopeAimWidget->RemoveFromParent();
			CurrentRopeAimWidget = nullptr;
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

void AArcher::DrawArrow(EHitType HitType, float Multiplier, int32 HitIdx, float LaunchDistance, float LaunchHeight)
{
	if (!HasAuthority()) return;

	// 이미 화살을 쥐고 있다면 중복 생성 방지
	if (CurrentArrow) return;

	CurrentArrow = GetArrowFromPool();
	if (CurrentArrow)
	{
		CurrentArrow->Prepare();
		CurrentArrow->SetAttackData(DamageEffectClass, Multiplier, HitType, HitIdx, LaunchDistance, LaunchHeight);
		
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

void AArcher::SetAimTargetLocation(const FVector& InLocation)
{
	AimTargetLocation = InLocation;
	bHasAimTarget = true;
}

void AArcher::ClearAimTarget()
{
	AimTargetLocation = FVector::ZeroVector;
	bHasAimTarget = false;
}
