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




// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

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
	if (AbilitySystemComponent && AbilitySystemComponent->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Event.BlockInput")))
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
	// 조준 중일 때는 질주 불가
	if (AbilitySystemComponent && AbilitySystemComponent->HasMatchingGameplayTag(FGameplayTags::Get().AimingStateTag))
	{
		return;
	}

	// 이미 질주 중이면 중복 추가 방지
	if (AbilitySystemComponent && AbilitySystemComponent->HasMatchingGameplayTag(FGameplayTags::Get().SprintStateTag))
	{
		return;
	}

	if (AttributeSet)
	{
		GetCharacterMovement()->MaxWalkSpeed = GetAttributeSet()->GetSprintSpeed();
	}
	
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->AddLooseGameplayTag(FGameplayTags::Get().SprintStateTag);
	}
}

void ACharacterBase::StopSprint()
{
	if (AttributeSet)
	{
		GetCharacterMovement()->MaxWalkSpeed = GetAttributeSet()->GetRunSpeed();
	}

	if (AbilitySystemComponent && AbilitySystemComponent->HasMatchingGameplayTag(FGameplayTags::Get().SprintStateTag))
	{
		AbilitySystemComponent->RemoveLooseGameplayTag(FGameplayTags::Get().SprintStateTag);
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
	EventData.EventTag = FGameplayTag::RequestGameplayTag(FName("Event.AbilitiesChanged"));
	EventData.Instigator = this;
	EventData.Target = this;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, EventData.EventTag, EventData);

}

void ACharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACharacterBase, AttributeSet);
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
