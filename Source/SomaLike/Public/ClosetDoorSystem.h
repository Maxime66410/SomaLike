// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionSystem.h"
#include "GameFramework/Actor.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "ClosetDoorSystem.generated.h"

UCLASS()
class SOMALIKE_API AClosetDoorSystem : public AActor, public IInteractionSystem
{
	GENERATED_BODY()

	UPROPERTY()
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	UPhysicsConstraintComponent* PhysicsConstraintComponent;

	UPROPERTY()
	bool bIsUsed = false;
	
public:	
	// Sets default values for this actor's properties
	AClosetDoorSystem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnInteract(ASomaLikeCharacter* Character) override;

	virtual void OnUse(ASomaLikeCharacter* Character) override;

	virtual void OnDrop(ASomaLikeCharacter* Character) override;

	UFUNCTION()
	void OpenDoor(float MouseX, float MouseY) const;

	UFUNCTION()
	bool GetIsUsed() const { return bIsUsed; }

	UFUNCTION()
	void SetIsUsed(bool LocalbIsUsed) { bIsUsed = LocalbIsUsed; }
};
