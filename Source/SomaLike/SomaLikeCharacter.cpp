// Copyright Epic Games, Inc. All Rights Reserved.

#include "SomaLikeCharacter.h"
#include "SomaLikeProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "InteractionSystem.h"
#include "Engine/LocalPlayer.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ASomaLikeCharacter

ASomaLikeCharacter::ASomaLikeCharacter()
{
	// Character doesnt have a rifle at start
	bHasRifle = false;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));


	// Interaction System
	PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));
}

void ASomaLikeCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

}

//////////////////////////////////////////////////////////////////////////// Input

void ASomaLikeCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASomaLikeCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASomaLikeCharacter::Look);

		// Interacting
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ASomaLikeCharacter::StartInteract);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ASomaLikeCharacter::TriggeredInteract);

		// Switching Mode
		EnhancedInputComponent->BindAction(SwitchMode, ETriggerEvent::Started, this, &ASomaLikeCharacter::SwitchModeInteract);

		// Dropping
		EnhancedInputComponent->BindAction(DropAction, ETriggerEvent::Started, this, &ASomaLikeCharacter::Drop);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


void ASomaLikeCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		if (!bCanMove) return;
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void ASomaLikeCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		if (!bCanLook) return;
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}

	// Get values from the mouse input and save for later
	LastMouseXPos = LookAxisVector.X;
	LastMouseYPos = LookAxisVector.Y;
}

void ASomaLikeCharacter::StartInteract(const FInputActionValue& Value)
{
	if(!bIsInteracting | !bIsInspect && !GetInteractable())
	{
		// Line trace to interact with objects
		FVector Start = FirstPersonCameraComponent->GetComponentLocation();
		FVector End = Start + FirstPersonCameraComponent->GetForwardVector() * 200.f;
		FHitResult Hit;
		FCollisionQueryParams TraceParams;
		TraceParams.AddIgnoredActor(this);
		GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECollisionChannel::ECC_Visibility, TraceParams);
		if (Hit.bBlockingHit)
		{
			if(IInteractionSystem* Interface = Cast<IInteractionSystem>(Hit.GetActor()))
			{
				Interface->OnInteract(this);
				Interface->OnEquip(this);
				Interface->OnInspect(this);
				Interface->OnUse(this);
			}
		}
	}
	else
	{
		if(PhysicsHandle->GrabbedComponent)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Dropping"));
			Drop(Value);
		}
	}
}

void ASomaLikeCharacter::TriggeredInteract(const FInputActionValue& Value)
{
	if(bIsInteracting && GetInteractable())
	{
		
	}
}


void ASomaLikeCharacter::Drop(const FInputActionValue& Value)
{
	if(bIsInteracting | bIsInspect && GetInteractable())
	{
		if(IInteractionSystem* Interface = Cast<IInteractionSystem>(GetInteractable()))
		{
			Interface->OnDrop(this);
		}
	}
}

void ASomaLikeCharacter::SwitchModeInteract(const FInputActionValue& Value)
{
	if(bIsInteracting | bIsInspect && GetInteractable())
	{
		SetSwitchMode(!GetSwitchMode());
		
		if(IInteractionSystem* Interface = Cast<IInteractionSystem>(GetInteractable()))
		{
			Interface->OnSwitchMode(this);
		}
	}
}

void ASomaLikeCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(bIsInteracting | bIsInspect && GetInteractable())
	{
		if(PhysicsHandle->GrabbedComponent)
		{
			float localDistance = bIsInspect ? 150.0f : 300.0f;
			FVector Start = Mesh1P->GetSocketLocation("head");
			FVector End = Start + (FirstPersonCameraComponent->GetForwardVector() * localDistance);
			PhysicsHandle->SetTargetLocation(End);

			// Set rotation of grabbed object to match the camera rotation
			if (!bIsInspect) PhysicsHandle->SetTargetRotation(FirstPersonCameraComponent->GetComponentRotation());
		}
		
		if(bIsInspect)
		{
			RotateObjectByMouse();
		}
	}
}

void ASomaLikeCharacter::RotateObjectByMouse()
{
	if(bIsInspect && GetInteractable())
	{
		// Rotate object by mouse
		float LocalMouseX = 0.f;
		float LocalMouseY = 0.f;
		GetWorld()->GetFirstPlayerController()->GetInputMouseDelta(LocalMouseX, LocalMouseY);
		FRotator NewRotation = FRotator(0.f, LocalMouseX, LocalMouseY);

		if(GetInteractable()->GetActorRotation() == NewRotation) return;
		
		GetInteractable()->AddActorLocalRotation(NewRotation);
	}
}

void ASomaLikeCharacter::SetHasRifle(bool bNewHasRifle)
{
	bHasRifle = bNewHasRifle;
}

bool ASomaLikeCharacter::GetHasRifle()
{
	return bHasRifle;
}

void ASomaLikeCharacter::SetInteractable(AActor* Interactable)
{
	CurrentInteractable = Interactable;
}

AActor* ASomaLikeCharacter::GetInteractable()
{
	return CurrentInteractable;
}

void ASomaLikeCharacter::SetInteracting(bool bNewInteracting)
{
	bIsInteracting = bNewInteracting;
}

bool ASomaLikeCharacter::GetInteracting()
{
	return bIsInteracting;
}

void ASomaLikeCharacter::SetInspect(bool bNewInspect)
{
	bIsInspect = bNewInspect;
}

bool ASomaLikeCharacter::GetInspect()
{
	return bIsInspect;
}

void ASomaLikeCharacter::SetSwitchMode(bool bNewSwitchMode)
{
	bIsSwitchMode = bNewSwitchMode;
}

bool ASomaLikeCharacter::GetSwitchMode()
{
	return bIsSwitchMode;
}

UPhysicsHandleComponent* ASomaLikeCharacter::GetPhysicsHandle()
{
	return PhysicsHandle;
}
