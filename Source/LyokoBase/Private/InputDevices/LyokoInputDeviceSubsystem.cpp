// Copyright © 2026 Lyoko - 96 l'Art Cheperdu

#include "InputDevices/LyokoInputDeviceSubsystem.h"
#include "CommonInputSubsystem.h"


void ULyokoInputDeviceSubsystem::Initialize(
    FSubsystemCollectionBase &Collection)
{
    Super::Initialize(Collection);

    UCommonInputSubsystem *CommonInput =
        UCommonInputSubsystem::Get(GetLocalPlayer());

    if (CommonInput)
    {
        CurrentInputType =
            CommonInput->GetCurrentInputType();


        CommonInput->OnInputMethodChangedNative
            .AddUObject(
                this,
                &ULyokoInputDeviceSubsystem::HandleInputMethodChanged);
    } 
    else 
    {
        UE_LOG(LogTemp, Error, TEXT("ULyokoInputDeviceSubsystem: CommonInput Subsystem not found for LocalPlayer %s"), *GetLocalPlayer()->GetName());
    }
}

void ULyokoInputDeviceSubsystem::HandleInputMethodChanged(
    ECommonInputType NewType)
{
    if (CurrentInputType == NewType)
    {
        return;
    }

    CurrentInputType = NewType;

    OnInputDeviceChanged.Broadcast(
        CurrentInputType);

    switch (CurrentInputType)
    {
        case ECommonInputType::MouseAndKeyboard:
            OnMouseKeyboardActive.Broadcast();
            break;
        case ECommonInputType::Gamepad:
            OnGamepadActive.Broadcast();
            break;
        default:
            break;
    }
}