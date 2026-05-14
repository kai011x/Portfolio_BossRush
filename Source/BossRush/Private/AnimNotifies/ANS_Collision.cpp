#include "AnimNotifies/ANS_Collision.h"
#include "Characters/CharacterBase.h"
void UANS_Collision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (MeshComp && MeshComp->GetOwner())
	{
		ACharacterBase* Character = Cast<ACharacterBase>(MeshComp->GetOwner());
		if (Character)
		{
			Character->SetCollisionShapesEnabled(true, HitType, DamageMultiplier, HitIdx, LaunchDistance, LaunchHeight);
		}
	}
}

void UANS_Collision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (MeshComp && MeshComp->GetOwner())
	{
		if (ACharacterBase* Character = Cast<ACharacterBase>(MeshComp->GetOwner()))
		{
			Character->SetCollisionShapesEnabled(false);
		}
	}
}
