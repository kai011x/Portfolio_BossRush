// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/AN_DrawArrow.h"
#include "Characters/Archer.h"

void UAN_DrawArrow::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp && MeshComp->GetOwner())
	{
		if (AArcher* Archer = Cast<AArcher>(MeshComp->GetOwner()))
		{
			Archer->DrawArrow(HitType, DamageMultiplier, HitIdx, LaunchDistance, LaunchHeight);
		}
	}
}
