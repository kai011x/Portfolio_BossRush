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

	UFUNCTION(BlueprintCallable, Category = "Aim")
	void StartAim();

	UFUNCTION(BlueprintCallable, Category = "Aim")
	void StopAim();


	virtual void OnNormalAttackInput() override;

	UFUNCTION(BlueprintCallable, Category = "Weapons")
	void DrawArrow(EHitType HitType = EHitType::None, float Multiplier = 1.0f, int32 HitIdx = 0, float LaunchDistance = 0.0f, float LaunchHeight = 0.0f);

	UFUNCTION(BlueprintCallable, Category = "Weapons")
	void ShootArrow();

	UFUNCTION(BlueprintCallable, Category = "GAS")
	AGameplayAbilityTargetActor* GetOrCreateTargetActor();

	void SetAimTargetLocation(const FVector& InLocation);
	void ClearAimTarget();

	// ?„ì ¯ ?œì–´ ?¨ìˆ˜
	void SetRopeAimWidgetVisible(bool bVisible);
	
	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void OnUpdateRopeAimUI(bool bValid, FVector Location);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	class UUserWidget* CurrentAimWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	class UUserWidget* CurrentRopeAimWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class URopeActionComponent* RopeActionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* RightFootSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* AttackMontage;

	ACArrow* GetArrowFromPool();

private:
	FVector AimTargetLocation;
	bool bHasAimTarget = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUserWidget> AimWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUserWidget> RopeAimWidgetClass;

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

protected:
	void UpdateCameraZoom(float DeltaTime);
};
