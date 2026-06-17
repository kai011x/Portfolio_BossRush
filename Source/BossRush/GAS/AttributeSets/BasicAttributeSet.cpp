// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/AttributeSets/BasicAttributeSet.h"
#include "GAS/Tags/GameplayTags.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "Characters/CharacterBase.h"
#include "Characters/Bosses/BossBase.h"

UBasicAttributeSet::UBasicAttributeSet()
{
	Health = 100.f;
	MaxHealth = 100.f;
	Stamina = 100.f;
	MaxStamina = 100.f;
}

void UBasicAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UBasicAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBasicAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UBasicAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBasicAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBasicAttributeSet, Resource, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBasicAttributeSet, MaxResource, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UBasicAttributeSet, DashCount, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBasicAttributeSet, MaxDashCount, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UBasicAttributeSet, Defense, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBasicAttributeSet, Attack, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBasicAttributeSet, RunSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBasicAttributeSet, SprintSpeed, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UBasicAttributeSet, DashStrength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBasicAttributeSet, Critical, COND_None, REPNOTIFY_Always);

}

void UBasicAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		const float LocalDamageDone = GetDamage();
		SetDamage(0.0f);

		if (LocalDamageDone > 0.0f)
		{
			const float NewHealth = GetHealth() - LocalDamageDone;
			SetHealth(FMath::Clamp(NewHealth, 0.0f, GetMaxHealth()));

			// 피격 리액션 및 대미지 기록 처리
			UAbilitySystemComponent* TargetASC = &Data.Target;
			ACharacterBase* TargetCharacter = Cast<ACharacterBase>(TargetASC->GetAvatarActor());
			
			// 공격자(Instigator) 정보 가져오기
			AActor* Instigator = Data.EffectSpec.GetContext().GetInstigator();
			ACharacterBase* AttackerCharacter = Cast<ACharacterBase>(Instigator);

			if (TargetCharacter)
			{
				const FGameplayTags& Tags = FGameplayTags::Get();

				// 보스인 경우 대미지 기록 (타겟 전환용)
				if (ABossBase* Boss = Cast<ABossBase>(TargetCharacter))
				{
					Boss->RecordDamage(AttackerCharacter, LocalDamageDone);
				}

				// SetByCaller에서 데이터 추출
				EHitType HitType = (EHitType)FMath::RoundToInt(Data.EffectSpec.GetSetByCallerMagnitude(Tags.HitTypeDataTag, false, 0.0f));
				int32 HitIdx = FMath::RoundToInt(Data.EffectSpec.GetSetByCallerMagnitude(Tags.HitIdxDataTag, false, 0.0f));
				float LaunchDist = Data.EffectSpec.GetSetByCallerMagnitude(Tags.LaunchDistanceDataTag, false, 0.0f);
				float LaunchHeight = Data.EffectSpec.GetSetByCallerMagnitude(Tags.LaunchHeightDataTag, false, 0.0f);

				TargetCharacter->PlayHitReact(HitType, HitIdx, LaunchDist, LaunchHeight, Instigator);
			}
		}
	}

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
	}
	else if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		SetStamina(FMath::Clamp(GetStamina(), 0.0f, GetMaxStamina()));
	}
	else if (Data.EvaluatedData.Attribute == GetResourceAttribute())
	{
		SetResource(FMath::Clamp(GetResource(), 0.0f, GetMaxResource()));
	}
	else if (Data.EvaluatedData.Attribute == GetDashCountAttribute())
	{
		SetDashCount(FMath::Clamp(GetDashCount(), 0.0f, GetMaxDashCount()));

		if (GetDashCount() >= GetMaxDashCount())
		{
			UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent();
			if (ASC)
			{
				FGameplayTagContainer TagContainer;
				TagContainer.AddTag(FGameplayTags::Get().DashRegenStatusTag);

				ASC->RemoveActiveEffectsWithGrantedTags(TagContainer);
			}
		}
	}
}

void UBasicAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
	else if (Attribute == GetStaminaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxStamina());
	}
	else if (Attribute == GetResourceAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxResource());
	}
	else if (Attribute == GetDashCountAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxDashCount());
	}
}
