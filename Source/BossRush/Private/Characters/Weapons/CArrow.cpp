// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Weapons/CArrow.h"
#include "GAS/Tags/GameplayTags.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "Characters/CharacterBase.h"

// Sets default values
ACArrow::ACArrow()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicateMovement(true);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(Mesh);
	Sphere->SetSphereRadius(5.f);
	Sphere->SetCollisionProfileName(TEXT("Projectile"));

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->SetUpdatedComponent(Mesh);
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
			if (IAbilitySystemInterface* TargetInterface = Cast<IAbilitySystemInterface>(OtherActor))
			{
				UAbilitySystemComponent* TargetASC = TargetInterface->GetAbilitySystemComponent();
				if (TargetASC && DamageEffectClass)
				{
					// 발사한 캐릭터(Owner)의 ASC 가져오기
					UAbilitySystemComponent* SourceASC = nullptr;
					if (IAbilitySystemInterface* SourceInterface = Cast<IAbilitySystemInterface>(GetOwner()))
					{
						SourceASC = SourceInterface->GetAbilitySystemComponent();
					}

					if (SourceASC)
					{
						FGameplayEffectContextHandle ContextHandle = SourceASC->MakeEffectContext();
						ContextHandle.AddInstigator(GetOwner(), this);
						ContextHandle.AddHitResult(Hit);

						FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, 1.0f, ContextHandle);
						if (SpecHandle.IsValid())
						{
							// 공격 배율 전달
							SpecHandle.Data.Get()->SetSetByCallerMagnitude(FGameplayTags::Get().DamageMultiplierDataTag, DamageMultiplier);

							// HitType 정보를 태그로 추가
							FString TypeTagString = FString::Printf(TEXT("Effect.HitType.%d"), (int32)HitType);
							SpecHandle.Data.Get()->AddDynamicAssetTag(FGameplayTag::RequestGameplayTag(*TypeTagString));

							// HitIdx 정보를 태그로 추가
							FString IdxTagString = FString::Printf(TEXT("Effect.HitIdx.%d"), HitIdx);
							SpecHandle.Data.Get()->AddDynamicAssetTag(FGameplayTag::RequestGameplayTag(*IdxTagString));

							SourceASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetASC);
						}
					}
				}
			}
		}

		Deactivate();
	}
}

