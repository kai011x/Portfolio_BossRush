// FighterAttributeSet.h

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"

#include "FighterAttributeSet.generated.h"

// 매크로 정의 (편의성)
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class BOSSRUSH_API UFighterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UFighterAttributeSet();

	// GetLifetimeReplicatedProps 오버라이드 필수
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	// --- Health ---
	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UFighterAttributeSet, Health);

	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UFighterAttributeSet, MaxHealth);

	// --- Stamina ---
	UPROPERTY(BlueprintReadOnly, Category = "Stamina", ReplicatedUsing = OnRep_Stamina)
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UFighterAttributeSet, Stamina);

	UPROPERTY(BlueprintReadOnly, Category = "Stamina", ReplicatedUsing = OnRep_MaxStamina)
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UFighterAttributeSet, MaxStamina);

	// --- Resource ---
	UPROPERTY(BlueprintReadOnly, Category = "Resource", ReplicatedUsing = OnRep_Resource)
	FGameplayAttributeData Resource;
	ATTRIBUTE_ACCESSORS(UFighterAttributeSet, Resource);

	UPROPERTY(BlueprintReadOnly, Category = "Resource", ReplicatedUsing = OnRep_MaxResource)
	FGameplayAttributeData MaxResource;
	ATTRIBUTE_ACCESSORS(UFighterAttributeSet, MaxResource);

	// --- Dash Count (오타 수정됨) ---
	UPROPERTY(BlueprintReadOnly, Category = "DashCount", ReplicatedUsing = OnRep_DashCount)
	FGameplayAttributeData DashCount;
	ATTRIBUTE_ACCESSORS(UFighterAttributeSet, DashCount);

	UPROPERTY(BlueprintReadOnly, Category = "DashCount", ReplicatedUsing = OnRep_MaxDashCount)
	FGameplayAttributeData MaxDashCount;
	ATTRIBUTE_ACCESSORS(UFighterAttributeSet, MaxDashCount);

	// --- Stats (ReplicatedUsing으로 변경 권장) ---
	// UI 업데이트를 위해 OnRep을 쓰는 것이 좋습니다.
	UPROPERTY(BlueprintReadOnly, Category = "Defense", ReplicatedUsing = OnRep_Defense)
	FGameplayAttributeData Defense;
	ATTRIBUTE_ACCESSORS(UFighterAttributeSet, Defense);

	UPROPERTY(BlueprintReadOnly, Category = "Attack", ReplicatedUsing = OnRep_Attack)
	FGameplayAttributeData Attack;
	ATTRIBUTE_ACCESSORS(UFighterAttributeSet, Attack);

	// 이동 속도는 값이 바뀌면 실제 무브먼트 컴포넌트에 적용해야 하므로 OnRep 필수
	UPROPERTY(BlueprintReadOnly, Category = "Speed", ReplicatedUsing = OnRep_RunSpeed)
	FGameplayAttributeData RunSpeed;
	ATTRIBUTE_ACCESSORS(UFighterAttributeSet, RunSpeed);

	UPROPERTY(BlueprintReadOnly, Category = "Speed", ReplicatedUsing = OnRep_SprintSpeed)
	FGameplayAttributeData SprintSpeed;
	ATTRIBUTE_ACCESSORS(UFighterAttributeSet, SprintSpeed);

public:
	// --- OnRep Functions ---

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldValue) const { GAMEPLAYATTRIBUTE_REPNOTIFY(UFighterAttributeSet, Health, OldValue); }

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldValue) const { GAMEPLAYATTRIBUTE_REPNOTIFY(UFighterAttributeSet, MaxHealth, OldValue); }

	UFUNCTION()
	void OnRep_Stamina(const FGameplayAttributeData& OldValue) const { GAMEPLAYATTRIBUTE_REPNOTIFY(UFighterAttributeSet, Stamina, OldValue); }

	UFUNCTION()
	void OnRep_MaxStamina(const FGameplayAttributeData& OldValue) const { GAMEPLAYATTRIBUTE_REPNOTIFY(UFighterAttributeSet, MaxStamina, OldValue); }

	UFUNCTION()
	void OnRep_Resource(const FGameplayAttributeData& OldValue) const { GAMEPLAYATTRIBUTE_REPNOTIFY(UFighterAttributeSet, Resource, OldValue); }

	UFUNCTION()
	void OnRep_MaxResource(const FGameplayAttributeData& OldValue) const { GAMEPLAYATTRIBUTE_REPNOTIFY(UFighterAttributeSet, MaxResource, OldValue); }

	UFUNCTION()
	void OnRep_DashCount(const FGameplayAttributeData& OldValue) const { GAMEPLAYATTRIBUTE_REPNOTIFY(UFighterAttributeSet, DashCount, OldValue); }

	UFUNCTION()
	void OnRep_MaxDashCount(const FGameplayAttributeData& OldValue) const { GAMEPLAYATTRIBUTE_REPNOTIFY(UFighterAttributeSet, MaxDashCount, OldValue); }

	// [추가] 기타 스탯용 OnRep
	UFUNCTION()
	void OnRep_Defense(const FGameplayAttributeData& OldValue) const { GAMEPLAYATTRIBUTE_REPNOTIFY(UFighterAttributeSet, Defense, OldValue); }

	UFUNCTION()
	void OnRep_Attack(const FGameplayAttributeData& OldValue) const { GAMEPLAYATTRIBUTE_REPNOTIFY(UFighterAttributeSet, Attack, OldValue); }

	UFUNCTION()
	void OnRep_RunSpeed(const FGameplayAttributeData& OldValue) const { GAMEPLAYATTRIBUTE_REPNOTIFY(UFighterAttributeSet, RunSpeed, OldValue); }

	UFUNCTION()
	void OnRep_SprintSpeed(const FGameplayAttributeData& OldValue) const { GAMEPLAYATTRIBUTE_REPNOTIFY(UFighterAttributeSet, SprintSpeed, OldValue); }
};