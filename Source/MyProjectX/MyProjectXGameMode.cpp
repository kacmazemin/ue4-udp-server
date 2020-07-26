// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyProjectXGameMode.h"
#include "MyProjectXPawn.h"
#include "MyProjectXHud.h"

AMyProjectXGameMode::AMyProjectXGameMode()
{
	DefaultPawnClass = AMyProjectXPawn::StaticClass();
	HUDClass = AMyProjectXHud::StaticClass();
}
