#include "Characters/Weapons/ACArrow_Skill.h"
#include "GAS/Tags/GameplayTags.h"
#include "NiagaraComponent.h"
#include "Components/SphereComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"

ACArrow_Skill::ACArrow_Skill()
{
	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	NiagaraComponent->SetupAttachment(RootComponent);
}

void ACArrow_Skill::BeginPlay()
{
	Super::BeginPlay();

	// 관통을 위해 Overlap 이벤트 바인딩
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ACArrow_Skill::OnOverlapBegin);
	
	// 중복 타격 방지 리스트 초기화
	AlreadyHitActors.Empty();
}

void ACArrow_Skill::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasAuthority()) return;

	// 자기 자신, 주인, 또는 이미 맞은 대상은 무시
	if (!OtherActor || OtherActor == this || OtherActor == GetOwner() || AlreadyHitActors.Contains(OtherActor))
	{
		return;
	}

	if (IAbilitySystemInterface* TargetInterface = Cast<IAbilitySystemInterface>(OtherActor))
	{
		UAbilitySystemComponent* TargetASC = TargetInterface->GetAbilitySystemComponent();
		if (TargetASC && DamageEffectClass)
		{
			// 발사한 캐릭터(Owner)의 ASC 가져오기
			UAbilitySystemComponent* SourceASC = nullptr;
			if (IAbilitySystemInterface* SourceInterface = Cast<IAbilitySystemInterface>(GetOwner()))
			{
				SourceASC = SourceInterface->GetAbilitySystemComponent();
			}

			if (SourceASC)
			{
				FGameplayEffectContextHandle ContextHandle = SourceASC->MakeEffectContext();
				ContextHandle.AddInstigator(GetOwner(), this);
				ContextHandle.AddHitResult(SweepResult);

				FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, 1.0f, ContextHandle);
				if (SpecHandle.IsValid())
				{
					// 공격 배율 및 HitType 전달
					SpecHandle.Data.Get()->SetSetByCallerMagnitude(FGameplayTags::Get().DamageMultiplierDataTag, DamageMultiplier);

					FString TypeTagString = FString::Printf(TEXT("Effect.HitType.%d"), (int32)HitType);
					SpecHandle.Data.Get()->AddDynamicAssetTag(FGameplayTag::RequestGameplayTag(*TypeTagString));

					// HitIdx 정보 추가
					FString IdxTagString = FString::Printf(TEXT("Effect.HitIdx.%d"), HitIdx);
					SpecHandle.Data.Get()->AddDynamicAssetTag(FGameplayTag::RequestGameplayTag(*IdxTagString));

					SourceASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetASC);
				}

				// 맞은 대상 리스트에 추가
				AlreadyHitActors.Add(OtherActor);
			}
		}
	}

	// 관통하지 않는 설정이면 여기서 제거
	if (!bPenetrate)
	{
		Deactivate();
	}
}

void ACArrow_Skill::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// 지형(StaticMesh)에 부딪혔을 때만 제거되도록 부모의 OnHit를 타겟팅해서 호출하거나 커스텀 로직 작성
	// 여기서는 단순히 지형 충돌 시 제거
	if (OtherComp && OtherComp->GetCollisionObjectType() == ECC_WorldStatic)
	{
		Deactivate();
	}
}
