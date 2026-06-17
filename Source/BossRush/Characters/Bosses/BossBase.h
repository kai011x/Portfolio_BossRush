// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterBase.h"
#include "Engine/DataTable.h"
#include "Engine/EngineTypes.h"
#include "BossBase.generated.h"


class UStateTreeComponent;
class UDataTable;

UENUM(BlueprintType)
enum class ETargetSwitchMethod : uint8
{
	Random,
	TopDamager,
	FarthestPlayer
};

UENUM(BlueprintType)
enum class EPatternType : uint8
{
	Attack      UMETA(DisplayName = "Attack"),      // 일반 근접 공격
	Dodge		UMETA(DisplayName = "Dodge"),		// 회피 대기 상태
	Defense      UMETA(DisplayName = "Defense"),    // 공격
	Max
};

UENUM(BlueprintType)
enum class EDistance : uint8
{
	Near    UMETA(DisplayName = "Near"),
	Middle  UMETA(DisplayName = "Middle"),
	Far     UMETA(DisplayName = "Far"),
	Max
};

USTRUCT(BlueprintType)
struct FPatternTypeList
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss|Pattern")
	TArray<EPatternType> PatternTypes;
};

USTRUCT(BlueprintType)
struct FBossMontageData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	EDistance AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	class UAnimMontage* Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	float PlayRate = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	int32 idx = 0;

};

/**
 * 보스 캐릭터들의 공통 기반 클래스
 */
UCLASS()
class BOSSRUSH_API ABossBase : public ACharacterBase
{
	GENERATED_BODY()

public:
	ABossBase();

	/** 현재 패턴 실행 중인지 여부 반환 */
	UFUNCTION(BlueprintCallable, Category = "Boss|Combat")
	bool IsExecutingPattern() const { return bIsExecutingPattern; }

	/** 기믹 실행 요청 */
	UFUNCTION(BlueprintCallable, Category = "Gimmick")
	void RequestGimmick(int32 GimmickIndex);

	/** 외부(AttributeSet)에서 대미지 기록을 위해 호출 */
	void RecordDamage(ACharacterBase* Attacker, float Amount);

	/** 특정 타입의 패턴 실행 */
	UFUNCTION(BlueprintCallable, Category = "Boss|Combat")
	void ExecutePattern();

	/** 다음 실행할 패턴 선택 및 반환 */
	UFUNCTION(BlueprintCallable, Category = "Boss|Combat")
	EPatternType GetNextPattern();

	/** 현재 타겟과의 거리에 따른 Enum 키 반환 */
	UFUNCTION(BlueprintCallable, Category = "Boss|Combat")
	EDistance GetCurrentDistanceKey() const;

	/** 데이터 테이블에서 각 거리별 몽타주 개수를 계산하여 기록 */
	void CalculateDistanceMontageCounts();

	/** 타겟을 향해 즉시 회전 */
	UFUNCTION(BlueprintCallable, Category = "Boss|Combat")
	void RotateToTarget();

public:

	/** 타겟 전환 로직 */
	void SwitchTarget();

	/** 어빌리티 종료 시 호출되는 콜백 */
	void OnAbilityEnded(const FAbilityEndedData& AbilityEndedData);

	/** 쿨타임 종료 후 실행 상태 리셋 */
	void ResetPatternExecution();

	virtual void BeginPlay() override;
protected:
	/** 현재 패턴 실행 중 여부 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Boss|Combat")
	bool bIsExecutingPattern = false;

	/** 패턴 사이의 지연 시간 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss|Combat")
	float PatternCooldown = 2.0f;

	/** 기믹 결과 처리 콜백 */
	UFUNCTION()
	void OnGimmickFinished(class AGimmickBase* Gimmick, bool bSuccess);

	/** StateTree 컴포넌트 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStateTreeComponent> StateTreeComponent;

public:

	/** 보스가 사용할 수 있는 기믹 클래스 목록 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss|Pattern")

	EPatternType CurPatternType;
	
	FTimerHandle CooldownTimerHandle;

	/** 보스가 사용할 수 있는 기믹 클래스 목록 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gimmick")
	TArray<TSubclassOf<class AGimmickBase>> GimmickClasses;

	/** 보스의 몽타주 패턴 데이터 테이블 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss|Combat", meta = (RequiredAssetDataTags = "RowStructure=/Script/BossRush.BossMontageData"))
	TObjectPtr<UDataTable> BossAttackMontageDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss|Combat", meta = (RequiredAssetDataTags = "RowStructure=/Script/BossRush.HittedData"))
	TObjectPtr<UDataTable> BossHittedMontageDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss|Pattern")
	TMap<EDistance, FPatternTypeList> DistancePatternMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Boss|Combat")
	TMap<EDistance, int32> DistanceMontageCounts;

	/* --- 패턴 배열 --- */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss|Combat")
	TArray<TSubclassOf<class UGA_PatternBase>> NearPatterns;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss|Combat")
	TArray<TSubclassOf<class UGA_PatternBase>> MiddlePatterns;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss|Combat")
	TArray<TSubclassOf<class UGA_PatternBase>> FarPatterns;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss|Combat")
	TArray<TSubclassOf<class UGA_AttackPatternBase>> NearAttacks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss|Combat")
	TArray<TSubclassOf<class UGA_AttackPatternBase>> MiddAttacks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss|Combat")
	TArray<TSubclassOf<class UGA_AttackPatternBase>> FarAttacks;

	/* --- 타겟팅 및 상태 변수 --- */
	UPROPERTY(BlueprintReadOnly, Category = "Boss|Combat")
	ACharacterBase* CurrentTarget;

	UPROPERTY(VisibleAnywhere, Category = "Boss|Combat")
	int32 PatternCounter = 0;

	UPROPERTY(VisibleAnywhere, Category = "Boss|Combat")
	float DamageAccumulatedSinceLastSwitch = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDistance DistKey = EDistance::Far;


	/** 플레이어별 누적 대미지 맵 */
	UPROPERTY()
	TMap<ACharacterBase*, float> PlayerDamageMap;

	const float DamageThresholdForSwitch = 1000.0f;
	const int32 PatternThresholdForSwitch = 3;
};


