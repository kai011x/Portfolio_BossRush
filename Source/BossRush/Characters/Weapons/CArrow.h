// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActionDatas.h"
#include "CArrow.generated.h"

class UStaticMeshComponent;
class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class BOSSRUSH_API ACArrow : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACArrow();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Deactivate();
	void Activate(const FVector& InLocation, const FRotator& InRotation);

	// 발사 전 준비 (손에 쥐기)
	void Prepare();
	// 실제 발사
	void Launch(const FVector& InDirection);

	FORCEINLINE bool IsInUse() const { return bInUse; }

	void SetAttackData(TSubclassOf<class UGameplayEffect> InEffectClass, float InMultiplier, EHitType InHitType, int32 InHitIdx = 0, float InLaunchDistance = 0.0f, float InLaunchHeight = 0.0f)
	{
		DamageEffectClass = InEffectClass;
		DamageMultiplier = InMultiplier;
		HitType = InHitType;
		HitIdx = InHitIdx;
		LaunchDistance = InLaunchDistance;
		LaunchHeight = InLaunchHeight;
	}

	void SetAttackData(float InMultiplier, EHitType InHitType, int32 InHitIdx = 0, float InLaunchDistance = 0.0f, float InLaunchHeight = 0.0f)
	{
		DamageMultiplier = InMultiplier;
		HitType = InHitType;
		HitIdx = InHitIdx;
		LaunchDistance = InLaunchDistance;
		LaunchHeight = InLaunchHeight;
	}

public:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* Sphere;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere, Category = "Settings")
	float LifeTime = 3.f;

protected:
	UPROPERTY()
	TSubclassOf<class UGameplayEffect> DamageEffectClass;

	float DamageMultiplier = 1.0f;
	EHitType HitType = EHitType::None;
	int32 HitIdx = 0;
	float LaunchDistance = 0.0f;
	float LaunchHeight = 0.0f;

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	TArray<AActor*> AlreadyHitActors;

private:
	FTimerHandle DeactivateTimerHandle;
	bool bInUse = false;

};
