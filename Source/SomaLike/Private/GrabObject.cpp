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
	
	Tags.Add(FName("Grabable"));
}

// Called when the game starts or when spawned
void AGrabObject::BeginPlay()
{
	Super::BeginPlay();

	InitialRotation = GetActorRotation();
	InitialLocation = GetActorLocation();
}

// Called every frame
void AGrabObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGrabObject::OnInteract(ASomaLikeCharacter* Character)
{
	IInteractionSystem::OnInteract(Character);

	if(!GetIsGrab())
	{
		UPhysicsHandleComponent* PhysicsHandle = Character->GetPhysicsHandle();
	
		PhysicsHandle->GrabComponentAtLocationWithRotation(MeshComponent, NAME_None, MeshComponent->GetComponentLocation(), MeshComponent->GetComponentRotation());
	
		Character->SetInteracting(true);

		SetIsGrab(true);

		Character->SetInteractable(this);
	}
}

void AGrabObject::OnDrop(ASomaLikeCharacter* Character)
{
	IInteractionSystem::OnDrop(Character);

	SetIsGrab(false);

	Character->SetInteracting(false);
	Character->SetInspect(false);
	Character->SetCanLook(true);
	Character->SetCanMove(true);
	Character->SetSwitchMode(false);
	
	MeshComponent->SetAngularDamping(0.0f);

	Character->SetInteractable(nullptr);

	Character->GetPhysicsHandle()->ReleaseComponent();
}

void AGrabObject::OnSwitchMode(ASomaLikeCharacter* Character)
{
	IInteractionSystem::OnSwitchMode(Character);

	if(Character->GetSwitchMode())
	{
		Character->SetInteracting(false);
		Character->SetInspect(true);
		Character->SetCanLook(false);
		Character->SetCanMove(false);

		UPhysicsHandleComponent* PhysicsHandle = Character->GetPhysicsHandle();
	
		PhysicsHandle->GrabComponentAtLocation(MeshComponent, NAME_None, MeshComponent->GetComponentLocation());

		MeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
		
		MeshComponent->SetAngularDamping(1000000000.0f);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("SSS"));
		Character->SetInteracting(true);
		Character->SetInspect(false);
		Character->SetCanLook(true);
		Character->SetCanMove(true);

		MeshComponent->SetAngularDamping(0.0f);

		UPhysicsHandleComponent* PhysicsHandle = Character->GetPhysicsHandle();
	
		PhysicsHandle->GrabComponentAtLocationWithRotation(MeshComponent, NAME_None, MeshComponent->GetComponentLocation(), MeshComponent->GetComponentRotation());
	
	}
}

void AGrabObject::OnResetPosition()
{
	IInteractionSystem::OnResetPosition();

	SetActorLocation(InitialLocation);
	SetActorRotation(InitialRotation);

	MeshComponent->SetPhysicsLinearVelocity(FVector::ZeroVector);
	MeshComponent->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
}

