// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/AN_ShootArrow.h"
#include "Characters/Archer.h"
#include "AbilitySystemComponent.h"
#include "Characters/Weapons/CArrow.h"

void UAN_ShootArrow::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp && MeshComp->GetOwner())
	{
		if (AArcher* Archer = Cast<AArcher>(MeshComp->GetOwner()))
		{
			if (UAbilitySystemComponent* ASC = Archer->GetAbilitySystemComponent())
			{
				if (Archer->CurrentArrow)
				{
					int32 FinalHitIdx = HitIdx;
					if (bRandomHitReaction)
					{
						FinalHitIdx = FMath::RandRange(0, 2);
					}
					
					Archer->CurrentArrow->SetAttackData(DamageMultiplier, HitType, FinalHitIdx, LaunchDistance, LaunchHeight);
				}

				if (ShootArrowClass)
				{
					ASC->TryActivateAbilityByClass(ShootArrowClass);
				}
			}
		}
	}
}
