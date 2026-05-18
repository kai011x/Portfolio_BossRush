// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Cues/GCN_FighterUltraBeam.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "GameFramework/Character.h"
#include "GAS/Tags/GameplayTags.h"

AGCN_FighterUltraBeam::AGCN_FighterUltraBeam()
{
	GameplayCueTag = FGameplayTags::Get().GameplaycueUltraSkillFighterBeamTag;
}

bool AGCN_FighterUltraBeam::OnActive_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
	Super::OnActive_Implementation(MyTarget, Parameters);

	ACharacter* TargetCharacter = Cast<ACharacter>(MyTarget);
	if (!TargetCharacter || !BeamSystem) return false;

	// 캐릭터의 Mesh에서 Muzzle_Front 소켓 위치에 Niagara 이펙트 생성 및 부착
	ActiveBeamComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
		BeamSystem,
		TargetCharacter->GetMesh(),
		MuzzleSocketName,
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		EAttachLocation::KeepRelativeOffset,
		true
	);

	return true;
}

bool AGCN_FighterUltraBeam::OnRemove_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
	if (ActiveBeamComponent)
	{
		ActiveBeamComponent->Deactivate();
		ActiveBeamComponent = nullptr;
	}

	return Super::OnRemove_Implementation(MyTarget, Parameters);
}
