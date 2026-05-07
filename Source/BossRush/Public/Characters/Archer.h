// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterBase.h"
#include "Archer.generated.h"

/**
 * 
 */
UCLASS()
class BOSSRUSH_API AArcher : public ACharacterBase
{
	GENERATED_BODY()

public:
	AArcher();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Aim")
	void StartAim();

	UFUNCTION(BlueprintCallable, Category = "Aim")
	void StopAim();


	virtual void OnNormalAttackInput() override;

private:
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
};
