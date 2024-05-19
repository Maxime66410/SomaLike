// Fill out your copyright notice in the Description page of Project Settings.


#include "ViewObject.h"

// Sets default values
AViewObject::AViewObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	SetRootComponent(MeshComponent);
	MeshComponent->SetCollisionProfileName(TEXT("PhysicsActor"));
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MeshComponent->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);

	Tags.Add(FName("Inspectable"));
}

// Called when the game starts or when spawned
void AViewObject::BeginPlay()
{
	Super::BeginPlay();

	InitialRotation = GetActorRotation();
	InitialLocation = GetActorLocation();
}

// Called every frame
void AViewObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AViewObject::OnInspect(ASomaLikeCharacter* Character)
{
	IInteractionSystem::OnInspect(Character);

	MeshComponent->SetSimulatePhysics(true);

	UPhysicsHandleComponent* PhysicsHandle = Character->GetPhysicsHandle();
	
	PhysicsHandle->GrabComponentAtLocation(MeshComponent, NAME_None, MeshComponent->GetComponentLocation());
	
	Character->SetInspect(true);

	Character->SetCanLook(false);
	Character->SetCanMove(false);

	MeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	SetInspected(true);

	Character->SetInteractable(this);

	MeshComponent->SetAngularDamping(100000.0f);

	SetActorRotation(InitialRotation);
	
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AViewObject::SetSimulatePhysicsMesh, .1f, false);
}

void AViewObject::OnDrop(ASomaLikeCharacter* Character)
{
	IInteractionSystem::OnDrop(Character);
	
	SetInspected(false);

	Character->SetInspect(false);

	Character->SetCanLook(true);
	Character->SetCanMove(true);
	
	MeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);

	MeshComponent->SetAngularDamping(0.0f);
	
	//ForceSetSimulatePhysicsMesh(true);

	Character->SetInteractable(nullptr);

	Character->GetPhysicsHandle()->ReleaseComponent();

	SetActorLocation(InitialLocation);
	SetActorRotation(InitialRotation);
	
	GetWorldTimerManager().ClearTimer(TimerHandle);
}

void AViewObject::SetSimulatePhysicsMesh()
{
	MeshComponent->SetSimulatePhysics(!MeshComponent->IsSimulatingPhysics());
	SetActorRotation(InitialRotation);
}

