// Fill out your copyright notice in the Description page of Project Settings.


#include "ClosetDoorSystem.h"

#include "GameFramework/PlayerState.h"

// Sets default values
AClosetDoorSystem::AClosetDoorSystem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(Root);
	MeshComponent->SetSimulatePhysics(true);
	MeshComponent->BodyInstance.bLockXTranslation = true;
	MeshComponent->BodyInstance.bLockYTranslation = true;
	MeshComponent->BodyInstance.bLockZTranslation = true;
	MeshComponent->BodyInstance.bLockYRotation = true;
	MeshComponent->BodyInstance.bLockXRotation = true;
	
	PhysicsConstraintComponent = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("PhysicsConstraintComponent"));
	PhysicsConstraintComponent->SetupAttachment(Root);
	PhysicsConstraintComponent->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
}

// Called when the game starts or when spawned
void AClosetDoorSystem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AClosetDoorSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AClosetDoorSystem::OnInteract(ASomaLikeCharacter* Character)
{
	IInteractionSystem::OnInteract(Character);

	if(!GetIsUsed())
	{
		SetIsUsed(true);

		Character->SetInteractable(this);

		Character->SetInteract(true);

		Character->SetCanLook(false);
		Character->SetCanMove(false);
	}
}

void AClosetDoorSystem::OnUse(ASomaLikeCharacter* Character)
{
	IInteractionSystem::OnUse(Character);

	if(GetIsUsed())
	{
		// Get the mouse position
		float MouseX = 0.0f;
		float MouseY = 0.0f;
		Character->GetPlayerState()->GetPlayerController()->GetInputMouseDelta(MouseX, MouseY);
		
		// MouseX and MouseY not equal to 0.0f
		if(MouseX == 0.0f && MouseY == 0.0f) return;

		// Open the door
		OpenDoor(MouseX, MouseY);
	}
}

void AClosetDoorSystem::OnDrop(ASomaLikeCharacter* Character)
{
	IInteractionSystem::OnDrop(Character);

	SetIsUsed(false);

	Character->SetInteractable(nullptr);

	Character->SetInteract(false);
	
	Character->SetCanLook(true);
	Character->SetCanMove(true);
}

void AClosetDoorSystem::OpenDoor(float MouseX, float MouseY) const
{
	MeshComponent->AddVelocityChangeImpulseAtLocation(FVector(MouseX, MouseY, 0.0f), MeshComponent->GetComponentLocation());
}

