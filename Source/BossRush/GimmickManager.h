// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GimmickBase.h"
#include "GimmickManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGimmickResultDelegate, AGimmickBase*, Gimmick, bool, bSuccess);

/**
 * 보스 기믹을 관리하고 실행하는 매니저 클래스 (싱글톤)
 */
UCLASS(Blueprintable, BlueprintType)
class BOSSRUSH_API UGimmickManager : public UObject
{
	GENERATED_BODY()

public:
	UGimmickManager();

	/** 싱글톤 인스턴스 반환 */
	UFUNCTION(BlueprintCallable, Category = "Gimmick", meta = (WorldContext = "WorldContextObject"))
	static UGimmickManager* Get(const UObject* WorldContextObject);

	/** 클래스 기반으로 기믹 액터를 스폰하고 실행 */
	UFUNCTION(BlueprintCallable, Category = "Gimmick", meta = (WorldContext = "WorldContextObject"))
	AGimmickBase* SpawnAndExecuteGimmick(TSubclassOf<AGimmickBase> GimmickClass, AActor* BossActor);

	/** 기믹이 결과를 매니저에게 보고할 때 사용 */
	void ReportGimmickResult(AGimmickBase* Gimmick, bool bSuccess);

	/** 기믹 결과를 구독할 델리게이트 */
	UPROPERTY(BlueprintAssignable, Category = "Gimmick")
	FGimmickResultDelegate OnGimmickResultReported;

private:
	static TObjectPtr<UGimmickManager> Instance;

	/** 현재 활성화된 기믹들 (여러 개일 수 있으므로 배열로 관리 가능) */
	UPROPERTY()
	TArray<AGimmickBase*> ActiveGimmicks;
};
