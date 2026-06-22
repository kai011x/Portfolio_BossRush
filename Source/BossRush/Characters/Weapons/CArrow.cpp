// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Weapons/CArrow.h"
#include "Engine/EngineTypes.h"
#include "GAS/Tags/GameplayTags.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "BossRushBlueprintLibrary.h"
#include "GameplayTagContainer.h"
#include "Characters/CharacterBase.h"

// Sets default values
ACArrow::ACArrow()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicateMovement(true);

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	RootComponent = Sphere;
	Sphere->SetSphereRadius(5.f);
	Sphere->SetCollisionProfileName(TEXT("Projectile"));
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Sphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	Sphere->SetUseCCD(true);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Sphere);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->SetUpdatedComponent(Sphere);
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.f;

}

// Called when the game starts or when spawned
void ACArrow::BeginPlay()
{
	Super::BeginPlay();
	
	Sphere->SetNotifyRigidBodyCollision(true);
	Sphere->OnComponentHit.AddDynamic(this, &ACArrow::OnHit);

	if (!bInUse)
	{
		Deactivate();
	}
}

// Called every frame
void ACArrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACArrow::Deactivate()
{
	if (!HasAuthority()) return;

	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	
	ProjectileMovement->Deactivate();
	ProjectileMovement->StopMovementImmediately();

	GetWorld()->GetTimerManager().ClearTimer(DeactivateTimerHandle);

	bInUse = false;
}

void ACArrow::Activate(const FVector& InLocation, const FRotator& InRotation)
{
	if (!HasAuthority()) return;

	SetActorLocationAndRotation(InLocation, InRotation);
	
	Prepare();
	Launch(InRotation.Vector());
}

void ACArrow::Prepare()
{
	if (!HasAuthority()) return;

	bInUse = true;

	SetActorHiddenInGame(false);
	SetActorEnableCollision(false); // 손에 쥐고 있을 때는 충돌 방지

	ProjectileMovement->Deactivate();
}

void ACArrow::Launch(const FVector& InDirection)
{
	if (!HasAuthority()) return;

	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	
	SetActorEnableCollision(true);

	if (AActor* MyOwner = GetOwner())
	{
		Sphere->IgnoreActorWhenMoving(MyOwner, true);
	}

	ProjectileMovement->Activate();
	ProjectileMovement->Velocity = InDirection * ProjectileMovement->InitialSpeed;

	GetWorld()->GetTimerManager().SetTimer(DeactivateTimerHandle, this, &ACArrow::Deactivate, LifeTime, false);
}

void ACArrow::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != this && OtherActor != GetOwner())
	{
		if (HasAuthority())
		{
			FHitInfo HitInfo;
			HitInfo.DamageMultiplier = DamageMultiplier;
			HitInfo.HitType = HitType;
			HitInfo.HitIdx = HitIdx;
			
			// 전역 함수 호출로 한 번에 해결!
			UBossRushBlueprintLibrary::ApplyDamageToTarget(GetOwner(), OtherActor, HitInfo);
			UE_LOG(LogTemp, Log, TEXT("Arrow hit %s with DamageMultiplier: %f, HitType: %d, HitIdx: %d"), *OtherActor->GetName(), DamageMultiplier, (int32)HitType, HitIdx);
		}

		Deactivate();
	}
}
