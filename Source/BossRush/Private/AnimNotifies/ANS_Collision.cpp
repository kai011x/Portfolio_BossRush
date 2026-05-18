#include "AnimNotifies/ANS_Collision.h"
#include "Characters/CharacterBase.h"

void UANS_Collision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (MeshComp && MeshComp->GetOwner())
	{
		UE_LOG(LogTemp, Warning, TEXT("[ANS_Collision] NotifyBegin on %s"), *MeshComp->GetOwner()->GetName());
		
		ACharacterBase* Character = Cast<ACharacterBase>(MeshComp->GetOwner());
		if (Character)
		{
			FHitInfo HitInfo(HitType, HitIdx, DamageMultiplier, LaunchDistance, LaunchHeight);
			Character->SetCollisionShapesEnabled(true, HitInfo);
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
