// Copyright © 2026 Lyoko - 96 l'Art Cheperdu


#include "Controllers/LyokoPlayerController.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "InputDevices/LyokoInputDeviceSubsystem.h"
#include "Framework/Application/SlateApplication.h"

void ALyokoPlayerController::BeginPlay()
{
    // Call the base class  
    Super::BeginPlay();

    ULocalPlayer *LocalPlayer = GetLocalPlayer();
    if (!LocalPlayer) return;

    ULyokoInputDeviceSubsystem *InputDeviceSystem = LocalPlayer->GetSubsystem<ULyokoInputDeviceSubsystem>();
    if (!InputDeviceSystem) return;

    InputDeviceSystem->OnMouseKeyboardActive.AddDynamic(this, &ALyokoPlayerController::OnMouseKeyboardActive);
    InputDeviceSystem->OnGamepadActive.AddDynamic(this, &ALyokoPlayerController::OnGamepadActive);

    switch (InputDeviceSystem->GetCurrentInputType())
    {
        case ECommonInputType::MouseAndKeyboard:
            OnMouseKeyboardActive();
            break;
        case ECommonInputType::Gamepad:
            OnGamepadActive();
            break;
        default:
            break;
    }
}


void ALyokoPlayerController::BindInputMapping(const TSoftObjectPtr<class UInputMappingContext>& InputMapping)
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

void ALyokoPlayerController::UnbindInputMapping(const TSoftObjectPtr<class UInputMappingContext>& InputMapping)
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

void ALyokoPlayerController::OnGamepadActive_Implementation()
{
    SetShowMouseCursor(false);
    FSlateApplication::Get().GetPlatformCursor()->Show(false);
}

void ALyokoPlayerController::OnMouseKeyboardActive_Implementation()
{
    SetShowMouseCursor(true);
    FSlateApplication::Get().GetPlatformCursor()->Show(true);
}
