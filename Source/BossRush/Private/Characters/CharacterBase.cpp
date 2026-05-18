// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/CharacterBase.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Abilities/GameplayAbility.h"
#include "Engine/ActorChannel.h" 
#include "GAS/Tags/GameplayTags.h"
#include "ActionDatas.h"
#include "Net/UnrealNetwork.h"
#include "Components/ShapeComponent.h"
#include "Components/SphereComponent.h"
#include "GAS/Effects/GE_Damage.h"
#include "Components/TargetingComponent.h"
#include "Components/WidgetComponent.h"

#include "BossRushBlueprintLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TargetingComponent = CreateDefaultSubobject<UTargetingComponent>(TEXT("TargetingComponent"));
	
	TargetWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("TargetWidget"));
	TargetWidget->SetupAttachment(GetMesh(), TEXT("spine_03"));
	TargetWidget->SetWidgetSpace(EWidgetSpace::Screen);
	TargetWidget->SetDrawAtDesiredSize(true);
	TargetWidget->SetVisibility(false);

	// 데미지 이펙트 기본값 설정
	DamageEffectClass = UGE_Damage::StaticClass();

	// Add the ability system component
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(ReplicationMode); 

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 500.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	CameraBoom->SocketOffset = FVector(0.0f, 0.0f, 150.0f); // 카메라를 약간 위로 올림

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm


}

UAbilitySystemComponent* ACharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
	// 자동으로 모든 ShapeComponent를 찾아서 등록 (본체 캡슐 제외)
	TArray<UShapeComponent*> AllShapes;
	GetComponents<UShapeComponent>(AllShapes);
	for (UShapeComponent* Shape : AllShapes)
	{
		if (Shape != GetCapsuleComponent())
		{
			CollisionShapes.AddUnique(Shape);
			UE_LOG(LogTemp, Warning, TEXT("[CharacterBase] Auto-Registered Collision Shape: %s"), *Shape->GetName());
		}
	}

	for (UShapeComponent* Shape : CollisionShapes)
	{
		if (Shape)
		{
			Shape->OnComponentBeginOverlap.AddDynamic(this, &ACharacterBase::OnAttackOverlapBegin);
			Shape->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}


void ACharacterBase::SetCollisionShapesEnabled(bool bEnabled, const FHitInfo& HitInfo)
{
	UE_LOG(LogTemp, Warning, TEXT("[CharacterBase] SetCollisionShapesEnabled: %s (HitIdx: %d)"), bEnabled ? TEXT("TRUE") : TEXT("FALSE"), HitInfo.HitIdx);

	CurrentHitInfo = HitInfo;

	if (bEnabled)
	{
		AlreadyHitActors.Empty();
	}

	for (UShapeComponent* Shape : CollisionShapes)
	{
		if (Shape)
		{
			if (bEnabled)
			{
				Shape->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
				Shape->SetCollisionResponseToAllChannels(ECR_Overlap);
			}
			else
			{
				Shape->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			}
		}
	}
}

void ACharacterBase::OnAttackOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == this || AlreadyHitActors.Contains(OtherActor)) return;

	UE_LOG(LogTemp, Warning, TEXT("[CharacterBase] Overlap Detected! Target: %s"), *OtherActor->GetName());

	AlreadyHitActors.Add(OtherActor);

	// 새로 만든 통합 함수 호출
	FHitInfo HitInfo = CurrentHitInfo;
	ApplyDamageToTarget(this, OtherActor, HitInfo);
}

void ACharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	if (AbilitySystemComponent)
	{
		if (AttributeSetClass && !AttributeSet)
		{
			// Create AttributeSet instance
			AttributeSet = NewObject<UBasicAttributeSet>(this, AttributeSetClass, TEXT("AttributeSet"));
			
			UE_LOG(LogTemp, Warning, TEXT("Created Class Type: %s"), *AttributeSet->GetClass()->GetName());
		
		}


		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		GrantAbilities(StartingAbilities);


		if (AttributeSet)
		{
			AbilitySystemComponent->AddSpawnedAttribute(AttributeSet);
		}
	}
}

void ACharacterBase::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
}



// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacterBase::BaseJump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACharacterBase::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACharacterBase::Look);

		// Dash
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Started, this, &ACharacterBase::Dash);
		
		//Normal Attack
		EnhancedInputComponent->BindAction(NormalAction, ETriggerEvent::Started, this, &ACharacterBase::OnNormalAttackInput);
	
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ACharacterBase::StartSprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ACharacterBase::StopSprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Canceled, this, &ACharacterBase::StopSprint);

		EnhancedInputComponent->BindAction(TargetingAction, ETriggerEvent::Started, this, &ACharacterBase::OnTargetingAction);
	}
} 

void ACharacterBase::OnTargetingAction()
{
	if (AbilitySystemComponent && TargetingAbilityClass)
	{
		// 이미 활성화되어 있다면 취소(토글 방식), 아니면 실행
		FGameplayTag TargetingTag = FGameplayTags::Get().TargetingStateTag;
		if (AbilitySystemComponent->HasMatchingGameplayTag(TargetingTag))
		{
			FGameplayTagContainer TargetTags;
			TargetTags.AddTag(TargetingTag);
			AbilitySystemComponent->CancelAbilities(&TargetTags);
		}
		else
		{
			AbilitySystemComponent->TryActivateAbilityByClass(TargetingAbilityClass);
		}
	}
}

void ACharacterBase::SetTargetingMode(bool bEnabled)
{
	if (!IsLocallyControlled()) return;

	bUseControllerRotationYaw = bEnabled;
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->bOrientRotationToMovement = !bEnabled;
	}
}

void ACharacterBase::SetTargetWidgetVisible_Implementation(bool bVisible)
{
	if (TargetWidget)
	{
		TargetWidget->SetVisibility(bVisible);
	}
}

void ACharacterBase::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void ACharacterBase::Move(const FInputActionValue& Value)
{
	if (!AbilitySystemComponent) return;

	if (AbilitySystemComponent->HasMatchingGameplayTag(FGameplayTags::Get().BlockInputEventTag) ||
		AbilitySystemComponent->HasMatchingGameplayTag(FGameplayTags::Get().RopeActionStateTag))
	{
		return;
	}

	CurrentInputVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, CurrentInputVector.Y);
		AddMovementInput(RightDirection, CurrentInputVector.X);
	}
}

void ACharacterBase::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ACharacterBase::StartSprint()
{
	if (!AbilitySystemComponent) return;

	// 이미 질주 중이면 중복 실행 방지
	if (AbilitySystemComponent->HasMatchingGameplayTag(FGameplayTags::Get().SprintStateTag))
	{
		return;
	}

	// 조준 중일 때는 질주 불가
	if (AbilitySystemComponent->HasMatchingGameplayTag(FGameplayTags::Get().AimingStateTag))
	{
		return;
	}

	// 타겟팅 중이면 강제 종료
	if (AbilitySystemComponent)
	{
		FGameplayTag TargetingTag = FGameplayTags::Get().TargetingStateTag;
		if (AbilitySystemComponent->HasMatchingGameplayTag(TargetingTag))
		{
			FGameplayTagContainer TargetTags;
			TargetTags.AddTag(TargetingTag);
			AbilitySystemComponent->CancelAbilities(&TargetTags);
		}
	}

	if (AbilitySystemComponent && SprintAbilityClass)
	{
		AbilitySystemComponent->TryActivateAbilityByClass(SprintAbilityClass);
	}
}

void ACharacterBase::StopSprint()
{
	if (AbilitySystemComponent)
	{
		FGameplayTag SprintTag = FGameplayTags::Get().SprintStateTag;
		FGameplayTagContainer TargetTags;
		TargetTags.AddTag(SprintTag);

		UE_LOG(LogTemp, Warning, TEXT("[CharacterBase] StopSprint Called. Has SprintTag: %s"), AbilitySystemComponent->HasMatchingGameplayTag(SprintTag) ? TEXT("True") : TEXT("False"));

		// SprintStateTag를 가진 모든 어빌리티(GA_Sprint)를 종료시킵니다.
		AbilitySystemComponent->CancelAbilities(&TargetTags);

		// 취소 호출 직후 상태 확인
		UE_LOG(LogTemp, Warning, TEXT("[CharacterBase] After CancelAbilities. Has SprintTag: %s"), AbilitySystemComponent->HasMatchingGameplayTag(SprintTag) ? TEXT("True") : TEXT("False"));
	}
}





void ACharacterBase::OnNormalAttackInput()
{
	if (!AbilitySystemComponent)
	{
		return;
	}

	if (HandleSprintAttack()) return;
	if (HandleDashAttack()) return;
	
	HandleNormalComboAttack();
}

bool ACharacterBase::HandleSprintAttack()
{
	if (AbilitySystemComponent->HasMatchingGameplayTag(FGameplayTags::Get().SprintStateTag))
	{
		if (DT_Montages && !GetCurrentMontage())
		{
			FMontageData* RowData = DT_Montages->FindRow<FMontageData>(FGameplayTags::Get().SprintAttackTag.GetTagName(), TEXT("SprintAttackLookup"));

			if (RowData && SprintAttackAbilityClass)
			{
				AbilitySystemComponent->TryActivateAbilityByClass(SprintAttackAbilityClass, true);
				return true;
			}
		}
	}
	return false;
}

bool ACharacterBase::HandleDashAttack()
{
	if (AbilitySystemComponent->HasMatchingGameplayTag(FGameplayTags::Get().DashStateTag))
	{
		if (DT_Montages && !GetCurrentMontage())
		{
			FMontageData* RowData = DT_Montages->FindRow<FMontageData>(FGameplayTags::Get().DashAttackTag.GetTagName(), TEXT("DashAttackLookup"));

			if (RowData && DashAttackAbilityClass)
			{
				AbilitySystemComponent->TryActivateAbilityByClass(DashAttackAbilityClass, true);
				return true;
			}
		}
	}
	return false;
}

void ACharacterBase::HandleNormalComboAttack()
{
	if (DT_NormalAttackCombo && NormalAttackAbilityClass)
	{
		FGameplayAbilitySpec* Spec = AbilitySystemComponent->FindAbilitySpecFromClass(NormalAttackAbilityClass);
		if (Spec && Spec->IsActive())
		{
			FGameplayEventData Payload;
			Payload.EventTag = FGameplayTags::Get().NormalAttackEventTag;
			Payload.Instigator = this;
			Payload.Target = this;

			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, FGameplayTags::Get().NormalAttackEventTag, Payload);
		}
		else
		{
			if (!GetCurrentMontage())
			{
				AbilitySystemComponent->TryActivateAbilityByClass(NormalAttackAbilityClass, true);
			}
		}
	}
}

TArray<FGameplayAbilitySpecHandle> ACharacterBase::GrantAbilities(TArray<TSubclassOf<UGameplayAbility>> AbilitiesToGrant)
{
	if (!AbilitySystemComponent || !HasAuthority())
	{
		return TArray<FGameplayAbilitySpecHandle>();
	}

	TArray<FGameplayAbilitySpecHandle> AbilityHandles;

	for (TSubclassOf<UGameplayAbility> Ability : AbilitiesToGrant)
	{
		FGameplayAbilitySpecHandle SpecHandle = AbilitySystemComponent->GiveAbility(
			FGameplayAbilitySpec(Ability, 1, -1, this));

		AbilityHandles.Add(SpecHandle);
	}

	SendAbilitiesChangedEvent();

	return AbilityHandles;
}

void ACharacterBase::RemoveAbilities(TArray<FGameplayAbilitySpecHandle> AbilityHandlesToRemove)
{
	if (!AbilitySystemComponent || !HasAuthority())
	{
		return;
	}

	for (FGameplayAbilitySpecHandle& SpecHandle : AbilityHandlesToRemove)
	{
		AbilitySystemComponent->ClearAbility(SpecHandle);
	}

	SendAbilitiesChangedEvent();
}

void ACharacterBase::SendAbilitiesChangedEvent()
{
	FGameplayEventData EventData;
	EventData.EventTag = FGameplayTags::Get().AbilitiesChangedEventTag;
	EventData.Instigator = this;
	EventData.Target = this;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, FGameplayTags::Get().AbilitiesChangedEventTag, EventData);

}

void ACharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACharacterBase, AttributeSet);
	DOREPLIFETIME(ACharacterBase, CharacterSize);
	DOREPLIFETIME(ACharacterBase, Identity);
}

bool ACharacterBase::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool bWroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	if (AttributeSet)
	{
		bWroteSomething |= Channel->ReplicateSubobject(AttributeSet, *Bunch, *RepFlags);
	}

	return bWroteSomething;
}

void ACharacterBase::ApplyRadialDamage(FVector Origin, float Radius, float Multiplier, EHitType HitType, int32 HitIdx, float LaunchDistance, float LaunchHeight)
{
	if (!HasAuthority()) return;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this);

	TArray<AActor*> OutActors;

	bool bHit = UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(),
		Origin,
		Radius,
		ObjectTypes,
		nullptr,
		IgnoreActors,
		OutActors
	);

	if (bHit)
	{
		for (AActor* TargetActor : OutActors)
		{
			FHitInfo HitInfo;
			HitInfo.DamageMultiplier = Multiplier;
			HitInfo.HitType = HitType;
			HitInfo.HitIdx = HitIdx;
			HitInfo.LaunchDistance = LaunchDistance;
			HitInfo.LaunchHeight = LaunchHeight;

			ApplyDamageToTarget(this, TargetActor, HitInfo);
		}
	}
}

void ACharacterBase::PlayHitReact(EHitType HitType, int32 HitIdx, float LaunchDistance, float LaunchHeight, AActor* HitInstigator)
{
	if (!DT_Hitted || !AbilitySystemComponent) return;

	// 크기 비교를 통한 시선 처리 로직
	if (HitInstigator)
	{
		ACharacterBase* Attacker = Cast<ACharacterBase>(HitInstigator);
		bool bShouldRotate = false;

		if (Attacker)
		{
			// 공격자와 사이즈가 같으면 바라봄
			if (Attacker->CharacterSize == CharacterSize)
			{
				bShouldRotate = true;
			}
			// 때린 캐릭터가 더 크면 바라보지 않음 (bShouldRotate = false 유지)
		}
		else
		{
			// 일반 액터(공격자가 CharacterBase가 아님)일 경우 기본적으로 바라보게 설정 (기존 로직 유지)
			if (CharacterSize != ESize::Big)
			{
				bShouldRotate = true;
			}
		}

		if (bShouldRotate)
		{
			FVector Direction = HitInstigator->GetActorLocation() - GetActorLocation();
			Direction.Z = 0.0f;
			if (!Direction.IsNearlyZero())
			{
				FRotator NewRot = FRotationMatrix::MakeFromX(Direction).Rotator();
				SetActorRotation(NewRot);
			}
		}
	}

	static const FString ContextString(TEXT("HitReactLookup"));
	TArray<FHittedData*> AllRows;
	DT_Hitted->GetAllRows<FHittedData>(ContextString, AllRows);

	for (FHittedData* Row : AllRows)
	{
		if (Row && Row->HitType == HitType && Row->idx == HitIdx)
		{
			if (Row->Montage)
			{
				float Duration = PlayAnimMontage(Row->Montage, Row->PlayRate);
				if (Duration > 0.0f)
				{
					// Character.State.Hitted 태그 추가
					FGameplayTag HittedTag = FGameplayTags::Get().HittedStateTag;
					AbilitySystemComponent->AddLooseGameplayTag(HittedTag);

					// 몽타주가 끝나면 태그 제거를 위한 타이머 설정
					FTimerHandle TimerHandle;
					GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([this, HittedTag]()
					{
						if (AbilitySystemComponent)
						{
							AbilitySystemComponent->RemoveLooseGameplayTag(HittedTag);
						}
					}), Duration, false);
				}
			}

			// 물리 효과 적용
			float FinalDist = (LaunchDistance > 0.0f) ? LaunchDistance : Row->LaunchDistance;
			float FinalHeight = (LaunchHeight > 0.0f) ? LaunchHeight : Row->LaunchHeight;

			if (FinalDist > 0.0f || FinalHeight > 0.0f)
			{
				FVector LaunchDir = -1 * GetActorForwardVector();
				LaunchDir.Z = 0.0f;
				LaunchDir.Normalize();

				FVector LaunchVelocity = LaunchDir * FinalDist;
				LaunchVelocity.Z = FinalHeight;

				LaunchCharacter(LaunchVelocity, true, true);
			}
			break;
		}
	}
}

void ACharacterBase::ApplyDamageToTarget(AActor* SourceActor, AActor* TargetActor, const FHitInfo& HitInfo)
{
	UBossRushBlueprintLibrary::ApplyDamageToTarget(SourceActor, TargetActor, HitInfo);
}

