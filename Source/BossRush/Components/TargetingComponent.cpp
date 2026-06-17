#include "Components/TargetingComponent.h"
#include "Characters/CharacterBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Controller.h"
#include "AbilitySystemComponent.h"

UTargetingComponent::UTargetingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false; // 타겟팅 시에만 틱 활성화
}

void UTargetingComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ACharacterBase>(GetOwner());
}

void UTargetingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!TargetCharacter || !OwnerCharacter)
	{
		EndTargeting();
		return;
	}

	// 거리 체크: 3000 이상 멀어지면 해제
	float Distance = FVector::Dist(OwnerCharacter->GetActorLocation(), TargetCharacter->GetActorLocation());
	if (Distance > Radius)
	{
		EndTargeting();
		return;
	}

	Tick_FollowTarget(DeltaTime);
}

void UTargetingComponent::StartTargeting()
{
	UE_LOG(LogTemp, Warning, TEXT("StartTargeting Called. Current Target: %s"), TargetCharacter ? *TargetCharacter->GetName() : TEXT("None"));

	if (IsTargeting())
	{
		EndTargeting();
		return;
	}

	FindTarget();

	if (TargetCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("Target Found: %s. Enabling Tick."), *TargetCharacter->GetName());
		SetComponentTickEnabled(true);
		
		// 로컬 플레이어인 경우에만 위젯을 표시합니다.
		if (OwnerCharacter && OwnerCharacter->IsLocallyControlled())
		{
			TargetCharacter->SetTargetWidgetVisible(true);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Target Found."));
	}
}

void UTargetingComponent::EndTargeting()
{
	UE_LOG(LogTemp, Warning, TEXT("EndTargeting Called."));
	if (TargetCharacter)
	{
		// 로컬 플레이어인 경우에만 위젯을 숨깁니다.
		if (OwnerCharacter && OwnerCharacter->IsLocallyControlled())
		{
			TargetCharacter->SetTargetWidgetVisible(false);
		}
	}

	TargetCharacter = nullptr;
	SetComponentTickEnabled(false);
	bMovingFocus = false;

	if (OwnerCharacter && OwnerCharacter->IsLocallyControlled())
	{
		OwnerCharacter->SetTargetingMode(false);
	}
}

void UTargetingComponent::FindTarget()
{
	if (!OwnerCharacter) return;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(OwnerCharacter);

	TArray<AActor*> OutActors;
	bool bHit = UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(),
		OwnerCharacter->GetActorLocation(),
		Radius,
		ObjectTypes,
		ACharacterBase::StaticClass(),
		IgnoreActors,
		OutActors
	);

	if (!bHit || OutActors.Num() <= 0) return;

	ACharacterBase* BestCandidate = nullptr;
	float HighestDot = -1.1f; // 내적 최소값(-1.0)보다 작게 시작하여 모든 후보를 수용

	for (AActor* Actor : OutActors)
	{
		ACharacterBase* Candidate = Cast<ACharacterBase>(Actor);
		if (!Candidate) continue;

		FVector DirToTarget = (Candidate->GetActorLocation() - OwnerCharacter->GetActorLocation()).GetSafeNormal();
		float Dot = FVector::DotProduct(OwnerCharacter->GetActorForwardVector(), DirToTarget);

		// 현재까지 발견된 적 중 가장 정면에 가까운(내적값이 큰) 적을 선택
		if (Dot > HighestDot)
		{
			HighestDot = Dot;
			BestCandidate = Candidate;
		}
	}

	TargetCharacter = BestCandidate;
}

void UTargetingComponent::Tick_FollowTarget(float DeltaTime)
{
	if (!OwnerCharacter || !TargetCharacter || !OwnerCharacter->GetController()) return;

	FRotator ControlRotation = OwnerCharacter->GetControlRotation();
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetActorLocation(), TargetCharacter->GetActorLocation());

	// 부드러운 회전
	if (ControlRotation.Equals(LookAtRotation, FinishAngle))
	{
		OwnerCharacter->GetController()->SetControlRotation(LookAtRotation);
		bMovingFocus = false;
	}
	else
	{
		FRotator NewRotation = UKismetMathLibrary::RInterpTo(ControlRotation, LookAtRotation, DeltaTime, InterpSpeed);
		OwnerCharacter->GetController()->SetControlRotation(NewRotation);
	}
}

void UTargetingComponent::ChangeTarget(bool bIsRight)
{
	if (!IsTargeting() || bMovingFocus) return;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(OwnerCharacter);
	IgnoreActors.Add(TargetCharacter);

	TArray<AActor*> OutActors;
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), OwnerCharacter->GetActorLocation(), Radius, ObjectTypes, ACharacterBase::StaticClass(), IgnoreActors, OutActors);

	ACharacterBase* BestCandidate = nullptr;
	float MinScore = 2.0f;

	for (AActor* Actor : OutActors)
	{
		ACharacterBase* Candidate = Cast<ACharacterBase>(Actor);
		if (!Candidate) continue;

		FVector DirToCandidate = (Candidate->GetActorLocation() - OwnerCharacter->GetActorLocation()).GetSafeNormal();
		FVector Cross = FVector::CrossProduct(OwnerCharacter->GetActorForwardVector(), DirToCandidate);
		float DotUp = FVector::DotProduct(Cross, OwnerCharacter->GetActorUpVector());

		// 오른쪽 타겟 변경이면 DotUp > 0, 왼쪽이면 DotUp < 0 인 대상 중 각도가 가장 작은 것 선택
		if (bIsRight ? (DotUp > 0.f) : (DotUp < 0.f))
		{
			if (FMath::Abs(DotUp) < MinScore)
			{
				MinScore = FMath::Abs(DotUp);
				BestCandidate = Candidate;
			}
		}
	}

	if (BestCandidate)
	{
		if (TargetCharacter)
		{
			// 로컬 플레이어인 경우에만 이전 위젯을 숨깁니다.
			if (OwnerCharacter && OwnerCharacter->IsLocallyControlled())
			{
				TargetCharacter->SetTargetWidgetVisible(false);
			}
		}

		TargetCharacter = BestCandidate;
		
		// 로컬 플레이어인 경우에만 새 위젯을 표시합니다.
		if (OwnerCharacter && OwnerCharacter->IsLocallyControlled())
		{
			TargetCharacter->SetTargetWidgetVisible(true);
		}
		
		bMovingFocus = true;
	}
}
