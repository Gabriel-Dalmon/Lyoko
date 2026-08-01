// Copyright � 2026 Lyoko - 96 l'Art Cheperdu
//     __                __       
//    / /   __  ______  / /______ 
//   / /   / / / / __ \/ //_/ __ \
//  / /___/ /_/ / /_/ / ,< / /_/ /
// /_____/\__, /\____/_/|_|\____/ 
//       /____/                   

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"

#include "CommonInputTypeEnum.h"

#include "LyokoInputDeviceSubsystem.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
    FOnInputDeviceChanged,
    ECommonInputType,
    NewDevice
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
    FOnMouseKeyboardActive
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
    FOnGamepadActive
);

//TODO@g Get rid of CommonInput dependency if possible by implementing a Slate InputProcessor ourselves
UCLASS()
class LYOKOBASE_API ULyokoInputDeviceSubsystem final :
    public ULocalPlayerSubsystem
{
    GENERATED_BODY()

public:

    virtual void Initialize(
        FSubsystemCollectionBase &Collection) override;

    UPROPERTY(BlueprintAssignable, Category = "Input")
    FOnInputDeviceChanged OnInputDeviceChanged;

    UPROPERTY(BlueprintAssignable, Category = "Input")
    FOnMouseKeyboardActive OnMouseKeyboardActive;

    UPROPERTY(BlueprintAssignable, Category = "Input")
    FOnGamepadActive OnGamepadActive;

    ECommonInputType GetCurrentInputType() const
    {
        return CurrentInputType;
    }

private:

    void HandleInputMethodChanged(
        ECommonInputType NewType);

private:

    ECommonInputType CurrentInputType;
};