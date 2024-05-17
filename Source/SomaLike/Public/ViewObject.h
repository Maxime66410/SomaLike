// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionSystem.h"
#include "GameFramework/Actor.h"
#include "ViewObject.generated.h"

UCLASS()
class SOMALIKE_API AViewObject : public AActor, public IInteractionSystem
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	bool bIsInspected = false;

	FTimerHandle TimerHandle;

	FRotator InitialRotation;
	FVector InitialLocation;
	
public:	
	// Sets default values for this actor's properties
	AViewObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void OnInspect(ASomaLikeCharacter* Character) override;

	virtual void OnDrop(ASomaLikeCharacter* Character) override;

	UFUNCTION()
	bool GetInspected() const { return bIsInspected; }

	UFUNCTION()
	void SetInspected(bool LocalbIsInspected) { bIsInspected = LocalbIsInspected; }

	UFUNCTION()
	void SetSimulatePhysicsMesh();

	UFUNCTION()
	void ForceSetSimulatePhysicsMesh(bool bSimulate) const { MeshComponent->SetSimulatePhysics(bSimulate); }
};
