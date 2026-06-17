#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"
#include "ActionDatas.generated.h"

/**
 * 피격 타입을 정의하는 열거형입니다.
 */
UENUM(BlueprintType)
enum class EHitType : uint8
{
	None,
	Light,
	Heavy,
	CrowdControl
};

USTRUCT(BlueprintType)
struct FHitInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EHitType HitType = EHitType::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 HitIdx = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LaunchDistance = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LaunchHeight = 0.0f;

	FHitInfo() {}
	FHitInfo(EHitType InType, int32 InIdx, float InMult, float InDist, float InHeight)
		: HitType(InType), HitIdx(InIdx), DamageMultiplier(InMult), LaunchDistance(InDist), LaunchHeight(InHeight) {}
};

/**
 * 범위 데미지 타입을 정의하는 열거형입니다.
 */
UENUM(BlueprintType)
enum class ERadialDamageType : uint8
{
	Instant     UMETA(DisplayName = "즉발형"),
	MultiHit    UMETA(DisplayName = "다단히트형")
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
	bool bCanStop = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	bool bFixedCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	float LaunchDistance = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	float LaunchHeight = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	class UFXSystemAsset* Effect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	FVector EffectLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	FVector EffectScale = FVector::OneVector;
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





USTRUCT(BlueprintType)
struct FHittedData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	EHitType HitType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	class UAnimMontage* Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	float PlayRate = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	int32 idx;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics")
	float LaunchDistance = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics")
	float LaunchHeight = 0.0f;
};
