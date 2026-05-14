#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TargetingComponent.generated.h"

class ACharacterBase;

/**
 * 적을 탐색하고 시선을 고정하는 타겟팅 시스템을 담당하는 컴포넌트입니다.
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BOSSRUSH_API UTargetingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTargetingComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void StartTargeting();
	void EndTargeting();
	void ChangeTarget(bool bIsRight);

	FORCEINLINE bool IsTargeting() const { return TargetCharacter != nullptr; }
	FORCEINLINE ACharacterBase* GetTarget() const { return TargetCharacter; }

protected:
	void FindTarget();
	void Tick_FollowTarget(float DeltaTime);

	UPROPERTY(EditAnywhere, Category = "Targeting")
	float Radius = 3000.0f;

	UPROPERTY(EditAnywhere, Category = "Targeting")
	float InterpSpeed = 15.0f;

	UPROPERTY(EditAnywhere, Category = "Targeting")
	float FinishAngle = 5.0f;

private:
	UPROPERTY()
	ACharacterBase* OwnerCharacter;

	UPROPERTY()
	ACharacterBase* TargetCharacter;

	bool bMovingFocus = false;
};
