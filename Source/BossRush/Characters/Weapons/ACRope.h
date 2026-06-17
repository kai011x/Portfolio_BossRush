// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ACRope.generated.h"

class UProjectileMovementComponent;
class UCableComponent;
class USphereComponent;
class UStaticMeshComponent;
class ACharacterBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRopeAttached);

UCLASS()
class BOSSRUSH_API ACRope : public AActor
{
	GENERATED_BODY()

public:
	ACRope();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	void Fire(const FVector& InTargetLocation);
	void Retract();

	bool IsActive() const { return bIsActive; }
	bool IsAttached() const { return bIsAttached; }

	UPROPERTY(BlueprintAssignable, Category = "Rope")
	FOnRopeAttached OnRopeAttached;

private:
	void UpdateRope(float DeltaTime);

	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCableComponent* CableComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* SphereComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* HookMeshComponent;

protected:
	UPROPERTY(EditAnywhere, Category = "Settings")
	float ThrowSpeed = 5000.0f;

	UPROPERTY(EditAnywhere, Category = "Settings")
	float MaxDistance = 4000.0f;

	UPROPERTY(EditAnywhere, Category = "Settings")
	FName RopeSocketName = TEXT("hand_r");

private:
	FVector TargetLocation;
	bool bIsActive = false;
	bool bIsAttached = false;
};
