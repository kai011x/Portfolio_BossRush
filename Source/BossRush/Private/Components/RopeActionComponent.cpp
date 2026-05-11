// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/RopeActionComponent.h"
#include "Characters/CharacterBase.h"
#include "Characters/Archer.h"
#include "Characters/Weapons/ACRope.h"
#include "AbilitySystemComponent.h"
#include "GAS/Tags/GameplayTags.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "Net/UnrealNetwork.h"

URopeActionComponent::URopeActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false; // 시작 시 Tick 꺼짐
	SetIsReplicatedByDefault(true);
}

void URopeActionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(URopeActionComponent, Rope);
}

void URopeActionComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacterBase>(GetOwner());

	// 로프 액터 미리 생성 (서버)
	if (OwnerCharacter && OwnerCharacter->HasAuthority() && RopeClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = OwnerCharacter;
		SpawnParams.Instigator = OwnerCharacter;
		Rope = GetWorld()->SpawnActor<ACRope>(RopeClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		
		if (Rope)
		{
			// 초기 부착 (회수 상태로 시작)
			Rope->Retract();
		}
	}
}

void URopeActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!OwnerCharacter) return;
	UAbilitySystemComponent* ASC = OwnerCharacter->GetAbilitySystemComponent();
	if (!ASC) return;

	// 1. 조준 단계: RopeAimStateTag가 있을 때
	if (ASC->HasMatchingGameplayTag(FGameplayTags::Get().RopeAimStateTag))
	{
		UpdateAimingLogic();
	}
	// 2. 이동 단계: RopeActionStateTag가 있을 때 (발사 후 대기 포함)
	else if (ASC->HasMatchingGameplayTag(FGameplayTags::Get().RopeActionStateTag))
	{
		UpdateMovementLogic(DeltaTime);
	}
	// [수정] 태그가 없다고 즉시 틱을 끄지 않음 (FinishMovement에서 명시적으로 종료)
}

void URopeActionComponent::SetAimingTickEnabled(bool bEnable)
{
	SetComponentTickEnabled(bEnable);
}

void URopeActionComponent::UpdateAimingLogic()
{
	if (!OwnerCharacter) return;

	FVector Start;
	FRotator Rotation;

	if (APlayerController* PC = Cast<APlayerController>(OwnerCharacter->GetController()))
	{
		PC->GetPlayerViewPoint(Start, Rotation);
	}
	else
	{
		Start = OwnerCharacter->GetActorLocation() + FVector(0, 0, OwnerCharacter->BaseEyeHeight);
		Rotation = OwnerCharacter->GetControlRotation();
	}

	FVector End = Start + (Rotation.Vector() * MaxDistance);

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(OwnerCharacter);

	bIsTargetValid = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);
	TargetLocation = bIsTargetValid ? Hit.ImpactPoint : End;

	// Archer 클래스를 통해 UI 업데이트
	if (AArcher* Archer = Cast<AArcher>(OwnerCharacter))
	{
		Archer->OnUpdateRopeAimUI(bIsTargetValid, TargetLocation);
	}
}

void URopeActionComponent::UpdateMovementLogic(float DeltaTime)
{
	if (!OwnerCharacter || !OwnerCharacter->GetCharacterMovement()) return;

	// 비행 모드에서만 이동 수행
	if (OwnerCharacter->GetCharacterMovement()->MovementMode != EMovementMode::MOVE_Flying) 
	{
		return;
	}

	FVector CurrentLocation = OwnerCharacter->GetActorLocation();
	FVector MoveDirection = (TargetLocation - CurrentLocation);
	float RemainingDistance = MoveDirection.Size();

	// 이미 목표 지점 부근이면 즉시 종료
	if (RemainingDistance <= StopDistance)
	{
		FinishMovement();
		return;
	}

	MoveDirection.Normalize();

	// 이번 프레임에 이동해야 할 거리
	float MoveStep = MoveSpeed * DeltaTime;

	// [중요] 정밀 도달 판정: 이번 프레임에 목표를 지나치게 된다면
	if (MoveStep >= RemainingDistance)
	{
		// 목표 지점으로 정확히 위치 고정 (Snapping)
		OwnerCharacter->SetActorLocation(TargetLocation, true);
		FinishMovement();
		return;
	}

	// 일반 이동: 방향과 속도에 따른 위치 오프셋 추가
	FHitResult SweepHit;
	OwnerCharacter->AddActorWorldOffset(MoveDirection * MoveStep, true, &SweepHit);
	
	// 이동 중 무언가에 부딪히면 종료
	if (SweepHit.bBlockingHit && SweepHit.GetActor() != OwnerCharacter)
	{
		FinishMovement();
	}
}

void URopeActionComponent::StartAiming()
{
	if (!OwnerCharacter || !AimAbilityClass) return;

	// [수정] 조준 시작 시 이전 데이터 초기화
	bIsTargetValid = false;
	TargetLocation = FVector::ZeroVector;

	UAbilitySystemComponent* ASC = OwnerCharacter->GetAbilitySystemComponent();
	if (ASC)
	{
		ASC->TryActivateAbilityByClass(AimAbilityClass);
	}
}

void URopeActionComponent::FinishAiming()
{
	if (!OwnerCharacter) return;

	UAbilitySystemComponent* ASC = OwnerCharacter->GetAbilitySystemComponent();
	if (!ASC) return;

	// 1. 조준 상태 태그 명시적 제거 (조준 UI 종료)
	ASC->RemoveLooseGameplayTag(FGameplayTags::Get().RopeAimStateTag);

	// 2. UI 즉시 제거 (RemoveFromParent 실행)
	if (AArcher* Archer = Cast<AArcher>(OwnerCharacter))
	{
		Archer->SetRopeAimWidgetVisible(false);
	}

	// 3. 조준 어빌리티 종료
	FGameplayTagContainer AimTags;
	AimTags.AddTag(FGameplayTags::Get().RopeAimStateTag);
	ASC->CancelAbilities(&AimTags);

	// 4. 조준 성공 시 액션 단계 진입
	if (bIsTargetValid)
	{
		ExecuteRopeAction();
	}
	else
	{
		SetComponentTickEnabled(false);
	}
}

void URopeActionComponent::ExecuteRopeAction()
{
	if (!OwnerCharacter) return;

	// 캐릭터를 타겟 위치로 즉시 회전 (Yaw만)
	FVector StartPos = OwnerCharacter->GetActorLocation();
	FVector TargetPos = TargetLocation;
	TargetPos.Z = StartPos.Z;

	FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(StartPos, TargetPos);
	OwnerCharacter->SetActorRotation(LookAtRot);

	UAbilitySystemComponent* ASC = OwnerCharacter->GetAbilitySystemComponent();
	if (ASC)
	{
		ASC->AddLooseGameplayTag(FGameplayTags::Get().RopeActionStateTag);
	}

	// 몽타주 재생 (실제 발사는 노티파이 AN_RopeThrow에서 FireRope 호출)
	if (FireMontage)
	{
		OwnerCharacter->PlayAnimMontage(FireMontage);
	}
	else
	{
		FireRope();
	}
}

void URopeActionComponent::FireRope()
{
	if (!OwnerCharacter || !Rope) return;

	Rope->OnRopeAttached.RemoveDynamic(this, &URopeActionComponent::OnRopeAttached);
	Rope->OnRopeAttached.AddDynamic(this, &URopeActionComponent::OnRopeAttached);

	Rope->Fire(TargetLocation);
}

void URopeActionComponent::OnRopeAttached()
{
	if (!OwnerCharacter || !Rope) return;

	Rope->OnRopeAttached.RemoveDynamic(this, &URopeActionComponent::OnRopeAttached);

	// [중요] 실제 갈고리가 박힌 위치로 타겟 업데이트
	TargetLocation = Rope->GetActorLocation();

	// [중요] 이동을 위해 틱을 강제로 활성화
	SetComponentTickEnabled(true);

	// 이동 물리 시작
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	
	if (MoveMontage)
	{
		OwnerCharacter->PlayAnimMontage(MoveMontage);
	}
}

void URopeActionComponent::FinishMovement()
{
	if (!OwnerCharacter) return;

	UAbilitySystemComponent* ASC = OwnerCharacter->GetAbilitySystemComponent();
	if (ASC)
	{
		ASC->RemoveLooseGameplayTag(FGameplayTags::Get().RopeActionStateTag);
	}

	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
	OwnerCharacter->StopAnimMontage(MoveMontage);

	if (Rope)
	{
		// [수정] 혹시라도 붙기 전에 취소된 경우를 대비해 여기서도 제거
		Rope->OnRopeAttached.RemoveDynamic(this, &URopeActionComponent::OnRopeAttached);
		Rope->Retract();
	}

	// 모든 로직이 종료되었으므로 Tick 비활성화
	SetComponentTickEnabled(false);
}
