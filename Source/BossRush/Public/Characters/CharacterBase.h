// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
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
#include "CharacterBase.generated.h"


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
	// 여러 개의 몽타주를 저장할 배열
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UAnimMontage*> Montages;
};

USTRUCT(BlueprintType)
struct FCharacterAnimMontages
{
	GENERATED_BODY()

public:
	// 상태(Key) - 몽타주(Value) 쌍으로 저장하는 Map
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TMap<ECharacterState, FMontageList> MontageMap;
};



UCLASS()
class BOSSRUSH_API ACharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ACharacterBase();

	// [추가 1] IAbilitySystemInterface 필수 구현 함수
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
	UInputAction* StrongAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* UltraSkill;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* DashAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SprintAction;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attribute")
	TSubclassOf<UBasicAttributeSet> AttributeSetClass;

	UPROPERTY(Replicated)
	UBasicAttributeSet* AttributeSet;

protected:
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void OnRep_PlayerState() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

	// [추가 2] ASC 컴포넌트 변수 선언
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



public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Dash")
	void Dash(const FInputActionValue& Value);

	UFUNCTION(BlueprintImplementableEvent, Category = "AttackAction")
	void OnNormalAttackInput();

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	TArray<FGameplayAbilitySpecHandle> GrantAbilities(TArray<TSubclassOf<UGameplayAbility>> AbilitiesToGrant);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void RemoveAbilities(TArray<FGameplayAbilitySpecHandle> AbilityHandlesToRemove);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void SendAbilitiesChangedEvent();


	UFUNCTION(BlueprintCallable, Category = "Attribute")
	UBasicAttributeSet* GetAttributeSet() const { return AttributeSet; }

protected:

	virtual void NotifyControllerChanged() override;
public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};
