// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/GA_Dash.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

UGA_Dash::UGA_Dash()
{
	// 기본 설정
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;


}


FVector UGA_Dash::GetDashDirection(AActor* SourceActor)
{
	ACharacter* Character = Cast<ACharacter>(SourceActor);
	if (!Character)
	{
		// 캐릭터가 아니면 그냥 액터의 정면 반환
		return SourceActor->GetActorForwardVector();
	}

	FVector InputVector = Character->GetLastMovementInputVector();
	
	if (InputVector.IsNearlyZero())
	{
		return Character->GetActorForwardVector();
	}

	return InputVector.GetSafeNormal();

}

void UGA_Dash::ResetRegenTimer()
{
	// 유효성 검사 (서버 권한 확인 등)
	if (!GetOwningActorFromActorInfo()->HasAuthority() || !DashRegenEffectClass)
	{
		return;
	}

	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (ASC)
	{
		// 1. 기존에 돌아가던 Regen 이펙트를 찾아서 제거 (타이머 강제 종료)
		// FGameplayEffectQuery::MakeQuery_MatchAllEffectTags() 등을 쓸 수도 있지만, 
		// 소스 이펙트 클래스로 지우는게 가장 확실합니다.
		ASC->RemoveActiveGameplayEffectBySourceEffect(DashRegenEffectClass, ASC);

		// 2. Regen 이펙트 다시 적용 (타이머 0초부터 재시작)
		FGameplayEffectContextHandle Context = ASC->MakeEffectContext();
		Context.AddSourceObject(this);

		FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(DashRegenEffectClass, 1.0f, Context);
		if (SpecHandle.IsValid())
		{
			ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}
