// Copyright Epic Games, Inc. All Rights Reserved.

#include "SomaLikeGameMode.h"
#include "SomaLikeCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASomaLikeGameMode::ASomaLikeGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Assets/Blueprint/Player/BP_FirstPersonCharacter.BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
