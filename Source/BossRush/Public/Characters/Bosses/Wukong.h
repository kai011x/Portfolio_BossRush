// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Bosses/BossBase.h"
#include "Wukong.generated.h"

/**
 * 서유기의 제천대성 우콩 보스 클래스
 */
UCLASS()
class BOSSRUSH_API AWukong : public ABossBase
{
	GENERATED_BODY()

public:
	AWukong();

	/** 여의봉 충돌체 가져오기 */
	FORCEINLINE class UCapsuleComponent* GetStaffCollision() const { return StaffCollision; }

protected:
	virtual void BeginPlay() override;

	/** 우콩 전용: 분신 소환 기믹 (예시) */
	UFUNCTION(BlueprintCallable, Category = "Wukong|Combat")
	void SpawnClones();

protected:
	/** 여의봉 충돌체 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* StaffCollision;

	/** 분신 소환 시 사용할 클래스 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wukong|Combat")
	TSubclassOf<AActor> CloneClass;

	/** 현재 소환된 분신 개수 */
	UPROPERTY(VisibleAnywhere, Category = "Wukong|Combat")
	int32 CurrentCloneCount = 0;
};
