// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GimmickBase.generated.h"

/**
 * 기믹의 종류를 정의하는 열거형
 */
UENUM(BlueprintType)
enum class EGimmickType : uint8
{
	Normal   UMETA(DisplayName = "Normal"),
	KeyInput UMETA(DisplayName = "KeyInput"),
	Destroy  UMETA(DisplayName = "Destroy"),
	Avoid    UMETA(DisplayName = "Avoid")
};

/**
 * 모든 보스 기믹의 기반이 되는 액터 클래스
 */
UCLASS(Abstract, Blueprintable)
class BOSSRUSH_API AGimmickBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AGimmickBase();

protected:
	virtual void BeginPlay() override;

public:	
	/** 기믹 시작 */
	UFUNCTION(BlueprintCallable, Category = "Gimmick")
	virtual void StartGimmick();

	/** 기믹 종료 (성공/실패 여부 전달) */
	UFUNCTION(BlueprintCallable, Category = "Gimmick")
	virtual void EndGimmick(bool bSuccess);

protected:
	/** 기믹 실패 시 페널티 적용 */
	UFUNCTION(BlueprintCallable, Category = "Gimmick")
	virtual void ApplyGimmickPenalty();

	/** 광역 피해 적용 헬퍼 */
	void ApplyRadialPenalty(float Damage, float Radius);

	/** 특정 타겟에게 디버프 적용 헬퍼 */
	void ApplyDebuffToTarget(ACharacterBase* Target, float SpeedMultiplier, float MaxHealthMultiplier, float Duration);

	/** 기믹과 관련된 플레이어들 중 랜덤하게 한 명 선택 */
	UFUNCTION(BlueprintCallable, Category = "Gimmick")
	ACharacterBase* GetRandomPlayer();

protected:
	/** 실패 시 적용할 광역 데미지 양 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gimmick|Penalty")
	float PenaltyDamage = 50.0f;

	/** 실패 시 적용할 광역 데미지 범위 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gimmick|Penalty")
	float PenaltyRadius = 1000.0f;

	/** 실패 시 적용할 디버프 지속 시간 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gimmick|Penalty")
	float DebuffDuration = 10.0f;

protected:
	/** 기믹 시작 시 호출되는 블루프린트 이벤트 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Gimmick")
	void OnGimmickStarted();

	/** 기믹 종료 시 호출되는 블루프린트 이벤트 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Gimmick")
	void OnGimmickEnded(bool bSuccess);

	/** 모든 플레이어의 입력을 제어하는 헬퍼 함수 */
	UFUNCTION(BlueprintCallable, Category = "Gimmick")
	void SetAllPlayersInputBlock(bool bBlock);

protected:
	/** 기믹 타입 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gimmick")
	EGimmickType GimmickType = EGimmickType::Normal;

	/** 기믹 제한 시간 (0이면 무제한) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gimmick")
	float TimeLimit = 0.0f;

	/** 기믹 실행에 관련된 보스 액터 */
	UPROPERTY(BlueprintReadOnly, Category = "Gimmick")
	AActor* OwnerBoss;

	FTimerHandle GimmickTimerHandle;
};
