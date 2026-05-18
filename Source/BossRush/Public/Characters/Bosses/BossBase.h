// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterBase.h"
#include "BossBase.generated.h"

UENUM(BlueprintType)
enum class ETargetSwitchMethod : uint8
{
	Random,
	TopDamager,
	FarthestPlayer
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

	/** 다음 패턴 실행 */
	UFUNCTION(BlueprintCallable, Category = "Boss|Combat")
	void ExecuteNextPattern();

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

	FTimerHandle CooldownTimerHandle;

	/** 기믹 결과 처리 콜백 */
	UFUNCTION()
	void OnGimmickFinished(class AGimmickBase* Gimmick, bool bSuccess);

public:
	/** 보스가 사용할 수 있는 기믹 클래스 목록 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gimmick")
	TArray<TSubclassOf<class AGimmickBase>> GimmickClasses;

	/* --- 패턴 배열 --- */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss|Combat")
	TArray<TSubclassOf<class UGA_PatternBase>> NearPatterns;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss|Combat")
	TArray<TSubclassOf<class UGA_PatternBase>> MiddlePatterns;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss|Combat")
	TArray<TSubclassOf<class UGA_PatternBase>> FarPatterns;

	/* --- 타겟팅 및 상태 변수 --- */
	UPROPERTY(BlueprintReadOnly, Category = "Boss|Combat")
	ACharacterBase* CurrentTarget;

	UPROPERTY(VisibleAnywhere, Category = "Boss|Combat")
	int32 PatternCounter = 0;

	UPROPERTY(VisibleAnywhere, Category = "Boss|Combat")
	float DamageAccumulatedSinceLastSwitch = 0.0f;

	/** 플레이어별 누적 대미지 맵 */
	UPROPERTY()
	TMap<ACharacterBase*, float> PlayerDamageMap;

	const float DamageThresholdForSwitch = 1000.0f;
	const int32 PatternThresholdForSwitch = 3;
};
