// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Bosses/Wukong.h"
#include "GAS/AttributeSets/WukongAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"

AWukong::AWukong()
{
	// 보스 기본 설정 (BossBase에서 설정한 것들 중 우콩 전용으로 덮어쓰기)
	Identity = EIdentity::Boss;

	// 우콩은 날렵하므로 쿨타임을 조금 짧게 설정
	PatternCooldown = 1.5f;

	// [중요] 우콩 전용 어트리뷰트 세트로 교체
	AttributeSetClass = UWukongAttributeSet::StaticClass();

	// 여의봉 충돌체 생성 및 소켓 부착
	StaffCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("StaffCollision"));
	StaffCollision->SetupAttachment(GetMesh(), TEXT("FX_Staff_Mid"));
	
	// 기본 충돌 설정
	StaffCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaffCollision->SetCapsuleHalfHeight(60.0f);
	StaffCollision->SetCapsuleRadius(10.0f);
}

void AWukong::BeginPlay()
{
	Super::BeginPlay();

	// 우콩 전용 어트리뷰트 초기화 (요력 등)
	if (GetAbilitySystemComponent())
	{
		const UWukongAttributeSet* WukongAS = Cast<UWukongAttributeSet>(GetAbilitySystemComponent()->GetAttributeSet(UWukongAttributeSet::StaticClass()));
		if (WukongAS)
		{
			// 쉴드 및 요력 초기 설정
			GetAbilitySystemComponent()->SetNumericAttributeBase(UBossAttributeSet::GetMaxShieldAttribute(), 500.0f);
			GetAbilitySystemComponent()->SetNumericAttributeBase(UBossAttributeSet::GetShieldAttribute(), 500.0f);
			
			GetAbilitySystemComponent()->SetNumericAttributeBase(UWukongAttributeSet::GetMaxMonkeyPowerAttribute(), 100.0f);
			GetAbilitySystemComponent()->SetNumericAttributeBase(UWukongAttributeSet::GetMonkeyPowerAttribute(), 0.0f);

			UE_LOG(LogTemp, Warning, TEXT("[Wukong] WukongAttributeSet Initialized. MonkeyPower: 0/100"));
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("[Wukong] The Monkey King has arrived with Special Powers!"));
}

void AWukong::SpawnClones()
{
	// 나중에 실제 분신 소환 로직 구현할 자리
	UE_LOG(LogTemp, Warning, TEXT("[Wukong] My clones will destroy you!"));
}
