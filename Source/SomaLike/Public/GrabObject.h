// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionSystem.h"
#include "GameFramework/Actor.h"
#include "GrabObject.generated.h"

UCLASS()
class SOMALIKE_API AGrabObject : public AActor, public IInteractionSystem
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	bool bIsGrab = false;

	FRotator InitialRotation;
	FVector InitialLocation;
	
public:	
	// Sets default values for this actor's properties
	AGrabObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnInteract(ASomaLikeCharacter* Character) override;

	virtual void OnDrop(ASomaLikeCharacter* Character) override;

	virtual void OnSwitchMode(ASomaLikeCharacter* Character) override;

	virtual void OnResetPosition() override;

	UFUNCTION()
	bool GetIsGrab() const { return bIsGrab; }

	UFUNCTION()
	void SetIsGrab(bool LocalbIsGrab) { bIsGrab = LocalbIsGrab; }
};
