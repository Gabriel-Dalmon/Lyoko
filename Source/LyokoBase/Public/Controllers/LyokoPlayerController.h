// Copyright © 2026 Lyoko - 96 l'Art Cheperdu
//     __                __       
//    / /   __  ______  / /______ 
//   / /   / / / / __ \/ //_/ __ \
//  / /___/ /_/ / /_/ / ,< / /_/ /
// /_____/\__, /\____/_/|_|\____/ 
//       /____/                   

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LyokoPlayerController.generated.h"

/**
 *
 */
UCLASS()
class LYOKOBASE_API ALyokoPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    //To move to PlayerState
    FTransform RespawnPoint;

public:
    virtual void BeginPlay() override;

protected:
    UFUNCTION(BlueprintCallable, Category = "Lyoko Player Controller")
    virtual void BindInputMapping(const TSoftObjectPtr<class UInputMappingContext>& InputMapping);

    UFUNCTION(BlueprintCallable, Category = "Lyoko Player Controller")
    virtual void UnbindInputMapping(const TSoftObjectPtr<class UInputMappingContext>& InputMapping);

protected:
    UFUNCTION(BlueprintNativeEvent, Category = "Input")
    void OnGamepadActive();
    virtual void OnGamepadActive_Implementation();

    UFUNCTION(BlueprintNativeEvent, Category = "Input")
    void OnMouseKeyboardActive();
    virtual void OnMouseKeyboardActive_Implementation();
};
