// Fill out your copyright notice in the Description page of Project Settings.


#include "AreaReset.h"

#include "InteractionSystem.h"

// Sets default values
AAreaReset::AAreaReset()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(BoxComponent);

	// Delegate begin overlap event
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AAreaReset::OnBeginOverlap);
}

// Called when the game starts or when spawned
void AAreaReset::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAreaReset::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor)
	{
		if(IInteractionSystem* Interface = Cast<IInteractionSystem>(OtherActor))
		{
			Interface->OnResetPosition();
		}
	}
}

// Called every frame
void AAreaReset::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

