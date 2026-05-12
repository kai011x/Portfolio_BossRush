// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Abilities/GameplayAbility.h"
#include "RopeActionComponent.generated.h"

class ACRope;
class ACharacterBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable )
class BOSSRUSH_API URopeActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	URopeActionComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	// 제어 인터페이스
	void SetAimingTickEnabled(bool bEnable);
	
	UFUNCTION(BlueprintCallable, Category = "Rope")
	void StartAiming();

	UFUNCTION(BlueprintCallable, Category = "Rope")
	void FinishAiming();

	// 애니메이션 노티파이용 인터페이스
	UFUNCTION(BlueprintCallable, Category = "Rope")
	void FireRope();

	// 데이터 관리
	void SetTargetLocation(const FVector& InLocation) { TargetLocation = InLocation; bIsTargetValid = true; }
	FVector GetTargetLocation() const { return TargetLocation; }
	bool IsTargetValid() const { return bIsTargetValid; }
	void FinishMovement();    // 이동 완료 및 Tick 종료
protected:
	UFUNCTION()
	void OnRopeAttached();

private:
	void UpdateAimingLogic();
	void UpdateMovementLogic(float DeltaTime);
	
	void ExecuteRopeAction(); // 발사 및 이동 시작


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rope")
	float MaxDistance = 4000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rope")
	float MoveSpeed = 3000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rope")
	float StopDistance = 150.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rope|Animation")
	UAnimMontage* FireMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rope|Animation")
	UAnimMontage* MoveMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rope")
	TSubclassOf<ACRope> RopeClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rope")
	TSubclassOf<UGameplayAbility> AimAbilityClass;

	UPROPERTY(Replicated)
	ACRope* Rope;

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UPROPERTY()
	ACharacterBase* OwnerCharacter;

	FVector TargetLocation;
	bool bIsTargetValid = false;
};
