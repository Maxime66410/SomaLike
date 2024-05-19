// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SomaLike/SomaLikeCharacter.h"
#include "UObject/Interface.h"
#include "InteractionSystem.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UInteractionSystem : public UInterface
{
	GENERATED_BODY()
};

class SOMALIKE_API IInteractionSystem
{
	GENERATED_BODY()
public:
	
	virtual void OnInspect(ASomaLikeCharacter* Character);

	virtual void OnInteract(ASomaLikeCharacter* Character);

	virtual void OnUse(ASomaLikeCharacter* Character);

	virtual void OnDrop(ASomaLikeCharacter* Character);

	virtual void OnEquip(ASomaLikeCharacter* Character);

	virtual void OnSwitchMode(ASomaLikeCharacter* Character);

	virtual void OnResetPosition();
};
