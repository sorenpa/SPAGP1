// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "SPAGP1.h"
#include "SPAGP1GameMode.h"
#include "SPAGP1Character.h"

ASPAGP1GameMode::ASPAGP1GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
