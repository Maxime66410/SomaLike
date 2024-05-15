// Fill out your copyright notice in the Description page of Project Settings.


#include "GrabObject.h"

// Sets default values
AGrabObject::AGrabObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	SetRootComponent(MeshComponent);
	MeshComponent->SetSimulatePhysics(true);
	MeshComponent->SetCollisionProfileName(TEXT("PhysicsActor"));
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MeshComponent->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);
}

// Called when the game starts or when spawned
void AGrabObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGrabObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGrabObject::OnInteract(ASomaLikeCharacter* Character)
{
	IInteractionSystem::OnInteract(Character);
	
	UPhysicsHandleComponent* PhysicsHandle = Character->GetPhysicsHandle();
	
	PhysicsHandle->GrabComponentAtLocationWithRotation(MeshComponent, NAME_None, MeshComponent->GetComponentLocation(), MeshComponent->GetComponentRotation());
	
	Character->SetInteracting(true);

	SetIsGrab(true);

	Character->SetInteractable(this);
}

void AGrabObject::OnDrop(ASomaLikeCharacter* Character)
{
	IInteractionSystem::OnDrop(Character);

	SetIsGrab(false);

	Character->SetInteracting(false);

	Character->SetInteractable(nullptr);

	Character->GetPhysicsHandle()->ReleaseComponent();
}

