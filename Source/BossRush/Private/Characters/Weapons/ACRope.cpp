// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/Weapons/ACRope.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "CableComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Characters/CharacterBase.h"

#include "Kismet/KismetMathLibrary.h"

ACRope::ACRope()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	RootComponent = SphereComponent;
	SphereComponent->SetCollisionProfileName(TEXT("Projectile"));
	SphereComponent->SetSphereRadius(10.0f);

	HookMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HookMesh"));
	HookMeshComponent->SetupAttachment(RootComponent);
	HookMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	CableComponent = CreateDefaultSubobject<UCableComponent>(TEXT("CableComponent"));
	CableComponent->SetupAttachment(RootComponent);
	CableComponent->CableWidth = 2.0f;
	CableComponent->NumSegments = 10;
	CableComponent->CableGravityScale = 0.0f;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent = RootComponent;
	ProjectileMovement->InitialSpeed = 0.0f;
	ProjectileMovement->MaxSpeed = ThrowSpeed;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.0f;
}

void ACRope::BeginPlay()
{
	Super::BeginPlay();
	
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ACRope::OnSphereOverlap);
	
	// 초기에는 비활성화
	Retract();
}

void ACRope::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsActive && !bIsAttached)
	{
		UpdateRope(DeltaTime);
	}
}

void ACRope::Fire(const FVector& InTargetLocation)
{
	ACharacterBase* OwnerCharacter = Cast<ACharacterBase>(GetOwner());
	if (!OwnerCharacter) return;

	// 발사 시 손에서 분리
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	TargetLocation = InTargetLocation;
	bIsActive = true;
	bIsAttached = false;

	// 케이블 시작점 설정 (캐릭터 소켓)
	CableComponent->SetVisibility(true);
	CableComponent->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, RopeSocketName);
	CableComponent->SetAttachEndToComponent(SphereComponent);

	// 발사 위치 설정
	FVector SpawnLocation = OwnerCharacter->GetMesh()->GetSocketLocation(RopeSocketName);
	SetActorLocation(SpawnLocation);
	
	FVector Direction = (TargetLocation - SpawnLocation).GetSafeNormal();
	ProjectileMovement->Velocity = Direction * ThrowSpeed;
	
	SetActorHiddenInGame(false);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ACRope::Retract()
{
	bIsActive = false;
	bIsAttached = false;
	ProjectileMovement->StopMovementImmediately();
	SetActorHiddenInGame(true);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CableComponent->SetVisibility(false);

	// 회수 시 다시 손에 부착
	if (ACharacterBase* OwnerCharacter = Cast<ACharacterBase>(GetOwner()))
	{
		AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, RopeSocketName);
	}
}

void ACRope::UpdateRope(float DeltaTime)
{
	ACharacterBase* OwnerCharacter = Cast<ACharacterBase>(GetOwner());
	if (OwnerCharacter)
	{
		float Distance = FVector::Dist(GetActorLocation(), OwnerCharacter->GetActorLocation());
		if (Distance > MaxDistance)
		{
			Retract();
		}
	}
}

void ACRope::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == GetOwner()||!bIsActive || bIsAttached) return;


	// 부착 성공
	bIsAttached = true;
	ProjectileMovement->StopMovementImmediately();
	
	if (OnRopeAttached.IsBound())
	{
		OnRopeAttached.Broadcast();
	}
}
