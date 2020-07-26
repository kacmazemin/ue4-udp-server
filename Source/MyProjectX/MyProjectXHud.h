// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MyProjectXHud.generated.h"

UCLASS(config = Game)
class AMyProjectXHud : public AHUD
{
	GENERATED_BODY()

public:
	AMyProjectXHud();

	/** Font used to render the vehicle info */
	UPROPERTY()
	UFont* HUDFont;

	// Begin AHUD interface
	virtual void DrawHUD() override;
	// End AHUD interface

};
