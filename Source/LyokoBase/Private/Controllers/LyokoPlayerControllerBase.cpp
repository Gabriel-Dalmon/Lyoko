// Copyright © 2025 Lyoko - 96 l'Art Cheperdu


#include "Controllers/LyokoPlayerControllerBase.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"


void ALyokoPlayerControllerBase::BindInputMapping(TSoftObjectPtr<class UInputMappingContext> InputMapping)
{
	ULocalPlayer *LocalPlayer = GetLocalPlayer();
	if (LocalPlayer == nullptr) return;

	UEnhancedInputLocalPlayerSubsystem *InputSystem =
		LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (InputSystem == nullptr) return;

	if (InputMapping.IsNull() == false)
	{
		InputSystem->AddMappingContext(
			InputMapping.LoadSynchronous(), 0);
	}
}
