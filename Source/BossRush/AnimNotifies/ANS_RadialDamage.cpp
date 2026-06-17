#include "AnimNotifies/ANS_RadialDamage.h"
#include "Characters/CharacterBase.h"

void UANS_RadialDamage::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	LastHitTime = 0.0f;

	if (DamageType == ERadialDamageType::Instant)
	{
		if (MeshComp && MeshComp->GetOwner())
		{
			if (ACharacterBase* Character = Cast<ACharacterBase>(MeshComp->GetOwner()))
			{
				Character->ApplyRadialDamage(Character->GetActorLocation(), Radius, DamageMultiplier, HitType, HitIdx, LaunchDistance, LaunchHeight);
			}
		}
	}
}

void UANS_RadialDamage::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (DamageType == ERadialDamageType::MultiHit)
	{
		LastHitTime += FrameDeltaTime;

		if (LastHitTime >= Interval)
		{
			LastHitTime = 0.0f;

			if (MeshComp && MeshComp->GetOwner())
			{
				if (ACharacterBase* Character = Cast<ACharacterBase>(MeshComp->GetOwner()))
				{
					Character->ApplyRadialDamage(Character->GetActorLocation(), Radius, DamageMultiplier, HitType, HitIdx, LaunchDistance, LaunchHeight);
				}
			}
		}
	}
}
