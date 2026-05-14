// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/AttributeSets/BasicAttributeSet.h"
#include "GAS/Tags/GameplayTags.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "Characters/CharacterBase.h"

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
	DOREPLIFETIME_CONDITION_NOTIFY(UBasicAttributeSet, MaxGroggy, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBasicAttributeSet, Groggy, COND_None, REPNOTIFY_Always);

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

			// 피격 리액션 처리
			UAbilitySystemComponent* TargetASC = &Data.Target;
			ACharacterBase* TargetCharacter = Cast<ACharacterBase>(TargetASC->GetAvatarActor());
			if (TargetCharacter)
			{
				EHitType HitType = EHitType::None;
				int32 HitIdx = 0;
				float LaunchDist = 0.0f;
				float LaunchHeight = 0.0f;
				
				// Spec에서 직접 태그 추출
				for (const FGameplayTag& Tag : Data.EffectSpec.GetDynamicAssetTags())
				{
					FString TagName = Tag.ToString();
					if (TagName.StartsWith(TEXT("Effect.HitType.")))
					{
						FString TypeIndexStr = TagName.RightChop(15);
						HitType = (EHitType)FCString::Atoi(*TypeIndexStr);
					}
					else if (TagName.StartsWith(TEXT("Effect.HitIdx.")))
					{
						FString IdxStr = TagName.RightChop(14);
						HitIdx = FCString::Atoi(*IdxStr);
					}
					else if (TagName.StartsWith(TEXT("Effect.Launch.Dist.")))
					{
						FString DistStr = TagName.RightChop(19);
						LaunchDist = (float)FCString::Atoi(*DistStr);
					}
					else if (TagName.StartsWith(TEXT("Effect.Launch.Height.")))
					{
						FString HeightStr = TagName.RightChop(21);
						LaunchHeight = (float)FCString::Atoi(*HeightStr);
					}
				}

				TargetCharacter->PlayHitReact(HitType, HitIdx, LaunchDist, LaunchHeight);
			}
		}
	}

	// GE       Clamp
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));

		// (�ɼ�) ��� ó�� ������ ���⿡ �߰� ����
		// if (GetHealth() <= 0.0f) { ... }
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

	// ���� ����Ǳ� ���� Max ���� ���� �ʵ��� Clamp
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
