// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimNotifies/ANS_Lookmaintarget.h"
#include "Characters/Bosses/BossBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Actor.h"

void UANS_Lookmaintarget::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (!MeshComp) return;

	AActor* OwnerActor = MeshComp->GetOwner();
	if (!OwnerActor) return;

	ABossBase* Boss = Cast<ABossBase>(OwnerActor);
	if (Boss)
	{
		Boss->RotateToTarget();
	}
}
