// Copyright � 2025-2027 Lyoko - 96 l'Art Cheperdu

#include "Controllers/LyokoPlayerController.h"
#include "InputDevices/LyokoInputDeviceSubsystem.h"
#include "Framework/Application/SlateApplication.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"

//----------------------------------------------------------------------------------------------------------------------
void ALyokoPlayerController::BeginPlay()
{
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

//----------------------------------------------------------------------------------------------------------------------
void ALyokoPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    BindInputMapping(SystemInputMapping);

    BindInputAction(PauseInputAction, &ALyokoPlayerController::TogglePause, ETriggerEvent::Started);
}

//----------------------------------------------------------------------------------------------------------------------
void ALyokoPlayerController::OnGamepadActive_Implementation()
{
    SetShowMouseCursor(false);
    FSlateApplication::Get().GetPlatformCursor()->Show(false);
}

//----------------------------------------------------------------------------------------------------------------------
void ALyokoPlayerController::OnMouseKeyboardActive_Implementation()
{
    SetShowMouseCursor(true);
    FSlateApplication::Get().GetPlatformCursor()->Show(true);
}

//----------------------------------------------------------------------------------------------------------------------
bool ALyokoPlayerController::SetGamePaused(bool bPaused)
{
    if (bIsGamePaused == bPaused)
    {
        return false;
    }

    bIsGamePaused = bPaused;

    if (bIsGamePaused)
    {
        OnGamePaused();
    }
    else {
        OnGameResumed();
    }

    return true;
}

//----------------------------------------------------------------------------------------------------------------------
void ALyokoPlayerController::TogglePause(const FInputActionValue &Value)
{
    SetGamePaused(!bIsGamePaused);
}
