// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterBase.h"
#include "Archer.generated.h"

class ACArrow;
class AGameplayAbilityTargetActor;

/**
 * 
 */
UCLASS()
class BOSSRUSH_API AArcher : public ACharacterBase
{
	GENERATED_BODY()

public:
	AArcher();

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Aim")
	void StartAim();

	UFUNCTION(BlueprintCallable, Category = "Aim")
	void StopAim();


	virtual void OnNormalAttackInput() override;

	UFUNCTION(BlueprintCallable, Category = "Weapons")
	void DrawArrow();

	UFUNCTION(BlueprintCallable, Category = "Weapons")
	void ShootArrow();

	UFUNCTION(BlueprintCallable, Category = "GAS")
	AGameplayAbilityTargetActor* GetOrCreateTargetActor();

	ACArrow* GetArrowFromPool();

	void SetAimTargetLocation(const FVector& InLocation) { AimTargetLocation = InLocation; bHasAimTarget = true; }
	void ClearAimTarget() { bHasAimTarget = false; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* AttackMontage;

private:
	FVector AimTargetLocation;
	bool bHasAimTarget = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AGameplayAbilityTargetActor> TargetActorClass;

	UPROPERTY()
	AGameplayAbilityTargetActor* TargetActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ACArrow> ArrowClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons", meta = (AllowPrivateAccess = "true"))
	FName ArrowSocketName = TEXT("ArrowSocket");

	UPROPERTY(Replicated)
	ACArrow* CurrentArrow;

	UPROPERTY()
	TArray<ACArrow*> ArrowPool;

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	const int32 ArrowPoolSize = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aim", meta = (AllowPrivateAccess = "true"))
	float AimArmLength = 150.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aim", meta = (AllowPrivateAccess = "true"))
	FVector AimSocketOffset = FVector(0.f, 50.f, 50.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aim", meta = (AllowPrivateAccess = "true"))
	float AimFOV = 70.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aim", meta = (AllowPrivateAccess = "true"))
	float AimInterpSpeed = 10.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Aim", meta = (AllowPrivateAccess = "true"))
	float DefaultArmLength;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Aim", meta = (AllowPrivateAccess = "true"))
	FVector DefaultSocketOffset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Aim", meta = (AllowPrivateAccess = "true"))
	float DefaultFOV;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Abilities")
	TSubclassOf<UGameplayAbility> AimAttackAbilityClass;
};
