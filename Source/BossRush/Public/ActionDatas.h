#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"
#include "ActionDatas.generated.h"
/**
 * 
 */

UENUM()
enum class EHittedType : uint8
{
	NormalHit, StrongHit, GroundHit, UpperHit, MAX
};

USTRUCT(BlueprintType)
struct FActionData : public FTableRowBase
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	class UAnimMontage* Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	float PlayRate = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	bool bCanMove = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	bool bFixedCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	class UFXSystemAsset* Effect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	FVector EffectLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	FVector EffectScale = FVector::OneVector;

};

USTRUCT(BlueprintType)
struct FHitData :public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	EHittedType HittedType = EHittedType::MAX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	int32 HitIndex = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	float PlayRate = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	float Power;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	float GroggyRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	float Launch = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	float StopTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	class USoundWave* Sound = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	class UFXSystemAsset* Effect = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	FVector EffectLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	FVector EffectScale = FVector::OneVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	FVector HitLocation = FVector::ZeroVector;
};


USTRUCT(BlueprintType)
struct FMontageData : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	FGameplayTag StateTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	class UAnimMontage* Montage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	float PlayRate = 1.0f;
};