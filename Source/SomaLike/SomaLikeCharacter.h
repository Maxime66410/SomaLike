// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "SomaLikeCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class ASomaLikeCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Interaction Ref **/
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* DropAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category =Interaction, meta = (AllowPrivateAccess = "true"))
	AActor* CurrentInteractable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category =Interaction, meta = (AllowPrivateAccess = "true"))
	bool bIsInteracting = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category =Interaction, meta = (AllowPrivateAccess = "true"))
	UPhysicsHandleComponent* PhysicsHandle;
	
public:
	ASomaLikeCharacter();

protected:
	virtual void BeginPlay();

public:
		
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	/** Bool for AnimBP to switch to another animation set */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	bool bHasRifle;

	/** Setter to set the bool */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	void SetHasRifle(bool bNewHasRifle);

	/** Getter for the bool */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	bool GetHasRifle();

	/* Interact Function */

	UFUNCTION(BlueprintCallable, Category = Interaction)
	void SetInteractable(AActor* Interactable);

	UFUNCTION(BlueprintCallable, Category = Interaction)
	AActor* GetInteractable();

	UFUNCTION(BlueprintCallable, Category = Interaction)
	void SetInteracting(bool bNewInteracting);

	UFUNCTION(BlueprintCallable, Category = Interaction)
	bool GetInteracting();

	UFUNCTION(BlueprintCallable, Category = Interaction)
	UPhysicsHandleComponent* GetPhysicsHandle();

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void StartInteract(const FInputActionValue& Value);
	
	void TriggeredInteract(const FInputActionValue& Value);

	void Drop(const FInputActionValue& Value);

	virtual void Tick(float DeltaSeconds) override;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

};

