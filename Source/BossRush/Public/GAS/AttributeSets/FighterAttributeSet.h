// FighterAttributeSet.h

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GAS/AttributeSets/BasicAttributeSet.h"

#include "FighterAttributeSet.generated.h"

// 매크로 정의 (편의성)
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS(Blueprintable, BlueprintType)
class BOSSRUSH_API UFighterAttributeSet : public UBasicAttributeSet
{
	GENERATED_BODY()

public:
	UFighterAttributeSet();

public:


};