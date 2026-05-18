// Fill out your copyright notice in the Description page of Project Settings.


#include "GimmickManager.h"

TObjectPtr<UGimmickManager> UGimmickManager::Instance = nullptr;

UGimmickManager::UGimmickManager()
{
}

UGimmickManager* UGimmickManager::Get(const UObject* WorldContextObject)
{
	if (!Instance && WorldContextObject)
	{
		Instance = NewObject<UGimmickManager>(GetTransientPackage(), UGimmickManager::StaticClass());
		Instance->AddToRoot(); // 가비지 컬렉션 방지
	}
	return Instance;
}

AGimmickBase* UGimmickManager::SpawnAndExecuteGimmick(TSubclassOf<AGimmickBase> GimmickClass, AActor* BossActor)
{
	if (!GimmickClass || !BossActor || !BossActor->GetWorld()) return nullptr;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = BossActor;
	SpawnParams.Instigator = Cast<APawn>(BossActor);

	AGimmickBase* NewGimmick = BossActor->GetWorld()->SpawnActor<AGimmickBase>(GimmickClass, BossActor->GetActorLocation(), BossActor->GetActorRotation(), SpawnParams);
	if (NewGimmick)
	{
		ActiveGimmicks.Add(NewGimmick);
		NewGimmick->StartGimmick();
	}

	return NewGimmick;
}

void UGimmickManager::ReportGimmickResult(AGimmickBase* Gimmick, bool bSuccess)
{
	if (!Gimmick) return;

	UE_LOG(LogTemp, Warning, TEXT("[GimmickManager] Gimmick %s reported success: %s"), *Gimmick->GetName(), bSuccess ? TEXT("TRUE") : TEXT("FALSE"));

	// 결과 알림
	OnGimmickResultReported.Broadcast(Gimmick, bSuccess);

	// 활성 목록에서 제거
	ActiveGimmicks.Remove(Gimmick);
}
