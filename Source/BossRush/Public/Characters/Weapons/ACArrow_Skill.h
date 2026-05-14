#pragma once

#include "CoreMinimal.h"
#include "Characters/Weapons/CArrow.h"
#include "ACArrow_Skill.generated.h"

class UNiagaraComponent;
class UNiagaraSystem;

/**
 * 나이아가라 이펙트를 부착하고 적을 관통하는 스킬용 투사체 클래스입니다.
 */
UCLASS()
class BOSSRUSH_API ACArrow_Skill : public ACArrow
{
	GENERATED_BODY()

public:
	ACArrow_Skill();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

public:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UNiagaraComponent* NiagaraComponent;

	UPROPERTY(EditAnywhere, Category = "Settings")
	bool bPenetrate = true;
};
