#include "AnimNotifies/AN_RopeThrow.h"
#include "Components/RopeActionComponent.h"
#include "Characters/CharacterBase.h"

void UAN_RopeThrow::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp || !MeshComp->GetOwner()) return;

	ACharacterBase* Character = Cast<ACharacterBase>(MeshComp->GetOwner());
	if (Character)
	{
		URopeActionComponent* RopeComp = Character->FindComponentByClass<URopeActionComponent>();
		if (RopeComp)
		{
			RopeComp->FireRope();
		}
	}
}
