// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GAS/AttributeSets/BasicAttributeSet.h"
#include "Engine/DataTable.h"
#include "ActionDatas.h"
#include "CharacterBase.generated.h"

class UGameplayAbility;
class UAnimMontage;
class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class UBasicAttributeSet;


UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	Idle		UMETA(DisplayName = "Idle"),
	Dash		UMETA(DisplayName = "Dash"),
	Attack		UMETA(DisplayName = "Attack"),
	Dead		UMETA(DisplayName = "Dead")
};

USTRUCT(BlueprintType)
struct FMontageList
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UAnimMontage*> Montages;
};

USTRUCT(BlueprintType)
struct FCharacterAnimMontages
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TMap<ECharacterState, FMontageList> MontageMap;
};



UCLASS()
class BOSSRUSH_API ACharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ACharacterBase();

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;



public:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* NormalAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SubAction; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* UltraSkill;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* NormalSkill;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* DashAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SprintAction;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* TargetingAction;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	ECharacterState CurrentState = ECharacterState::Idle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	FCharacterAnimMontages MontageSet;

public:
	UPROPERTY(BlueprintReadOnly, Category = "Input")
	FVector2D CurrentInputVector;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	UDataTable* DT_NormalAttackCombo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	UDataTable* DT_StrongAttackCombo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	UDataTable* DT_Montages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	UDataTable* DT_Skills;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	UDataTable* DT_Hitted;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attribute")
	TSubclassOf<UBasicAttributeSet> AttributeSetClass;

	UPROPERTY(Replicated)
	UBasicAttributeSet* AttributeSet;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TArray<class UShapeComponent*> CollisionShapes;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UTargetingComponent* TargetingComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* TargetWidget;

public:
	UFUNCTION(BlueprintNativeEvent, Category = "Targeting")
	void SetTargetWidgetVisible(bool bVisible);
	virtual void SetTargetWidgetVisible_Implementation(bool bVisible);

	UFUNCTION(BlueprintCallable, Category = "Targeting")
	class UTargetingComponent* GetTargetingComponent() const { return TargetingComponent; }

protected:
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void OnRep_PlayerState() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	UAbilitySystemComponent* AbilitySystemComponent;
	

public:



protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	EGameplayEffectReplicationMode ReplicationMode = EGameplayEffectReplicationMode::Mixed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartingAbilities;

 
public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void StartSprint();
	
	void StopSprint();

	void OnTargetingAction();

	virtual void SetTargetingMode(bool bEnabled);

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Dash")
	void Dash(const FInputActionValue& Value);

	UFUNCTION(BlueprintImplementableEvent, Category = "Jump")
	void BaseJump(const FInputActionValue& Value);
UFUNCTION(BlueprintCallable, Category = "AttackAction")
virtual void OnNormalAttackInput();

protected:
// --- 공격 타입별 헬퍼 함수 ---
virtual bool HandleSprintAttack();
virtual bool HandleDashAttack();
virtual void HandleNormalComboAttack();

public:
UFUNCTION(BlueprintCallable, Category = "Abilities")

	TArray<FGameplayAbilitySpecHandle> GrantAbilities(TArray<TSubclassOf<UGameplayAbility>> AbilitiesToGrant);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void RemoveAbilities(TArray<FGameplayAbilitySpecHandle> AbilityHandlesToRemove);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void SendAbilitiesChangedEvent();


	UFUNCTION(BlueprintCallable, Category = "Attribute")
	UBasicAttributeSet* GetAttributeSet() const { return AttributeSet; }

public:
	/* --- Ability Classes --- */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Abilities")
	TSubclassOf<UGameplayAbility> SprintAbilityClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Abilities")
	TSubclassOf<UGameplayAbility> TargetingAbilityClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Abilities")
	TSubclassOf<UGameplayAbility> SprintAttackAbilityClass; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Abilities")
	TSubclassOf<UGameplayAbility> DashAttackAbilityClass;  

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Abilities")
	TSubclassOf<UGameplayAbility> NormalAttackAbilityClass;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void SetCollisionShapesEnabled(bool bEnabled, EHitType HitType = EHitType::None, float Multiplier = 1.0f, int32 HitIdx = 0, float LaunchDistance = 0.0f, float LaunchHeight = 0.0f);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void ApplyRadialDamage(FVector Origin, float Radius, float Multiplier, EHitType HitType, int32 HitIdx = 0, float LaunchDistance = 0.0f, float LaunchHeight = 0.0f);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void PlayHitReact(EHitType HitType, int32 HitIdx = 0, float LaunchDistance = 0.0f, float LaunchHeight = 0.0f);


protected:

	virtual void NotifyControllerChanged() override;

	UFUNCTION()
	void OnAttackOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	TSubclassOf<class UGameplayEffect> DamageEffectClass;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	EHitType CurrentHitType;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	int32 CurrentHitIdx;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	float CurrentLaunchDistance;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	float CurrentLaunchHeight;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	float CurrentDamageMultiplier = 1.0f;

	TArray<AActor*> AlreadyHitActors;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};
