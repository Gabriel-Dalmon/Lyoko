// Copyright © 2025 Lyoko - 96 l'Art Cheperdu


#include "Controllers/LyokoPlayerControllerBase.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"


void ALyokoPlayerControllerBase::BindInputMapping(const TSoftObjectPtr<class UInputMappingContext>& InputMapping)
{
	ULocalPlayer *LocalPlayer = GetLocalPlayer();
	if (!LocalPlayer) return;

	UEnhancedInputLocalPlayerSubsystem *InputSystem =
		LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (!InputSystem) return;

	if (!InputMapping.IsNull() || !InputSystem->HasMappingContext(InputMapping.LoadSynchronous()))
	{
		InputSystem->AddMappingContext(
			InputMapping.LoadSynchronous(), 0);
	}
}

void ALyokoPlayerControllerBase::UnbindInputMapping(const TSoftObjectPtr<class UInputMappingContext>& InputMapping)
{
	ULocalPlayer *LocalPlayer = GetLocalPlayer();
	if (!LocalPlayer) return;

	UEnhancedInputLocalPlayerSubsystem *InputSystem =
		LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (!InputSystem) return;

	if (!InputMapping.IsNull() && InputSystem->HasMappingContext(InputMapping.LoadSynchronous()))
	{
		InputSystem->RemoveMappingContext(
			InputMapping.LoadSynchronous());
	}
}
