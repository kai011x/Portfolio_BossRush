// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Fighter.h"
#include "Components/SphereComponent.h"

AFighter::AFighter()
{
	RightHandSphere = CreateDefaultSubobject<USphereComponent>(TEXT("RightHandSphere"));
	RightHandSphere->SetupAttachment(GetMesh(), TEXT("hand_r"));
	RightHandSphere->SetSphereRadius(10.0f);
	RightHandSphere->SetCollisionProfileName(TEXT("NoCollision"));

	LeftHandSphere = CreateDefaultSubobject<USphereComponent>(TEXT("LeftHandSphere"));
	LeftHandSphere->SetupAttachment(GetMesh(), TEXT("hand_l"));
	LeftHandSphere->SetSphereRadius(10.0f);
	LeftHandSphere->SetCollisionProfileName(TEXT("NoCollision"));

	RightFootSphere = CreateDefaultSubobject<USphereComponent>(TEXT("RightFootSphere"));
	RightFootSphere->SetupAttachment(GetMesh(), TEXT("foot_r"));
	RightFootSphere->SetSphereRadius(10.0f);
	RightFootSphere->SetCollisionProfileName(TEXT("NoCollision"));

	CollisionShapes.Add(RightHandSphere);
	CollisionShapes.Add(LeftHandSphere);
	CollisionShapes.Add(RightFootSphere);
}

