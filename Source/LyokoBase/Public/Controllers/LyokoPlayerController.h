// Copyright © 2025-2027 Lyoko - 96 l'Art Cheperdu
//     __                __       
//    / /   __  ______  / /______ 
//   / /   / / / / __ \/ //_/ __ \
//  / /___/ /_/ / /_/ / ,< / /_/ /
// /_____/\__, /\____/_/|_|\____/ 
//       /____/                   

#pragma once

//----------------------------------------------------------------------------------------------------------------------
#include "CoreMinimal.h"
#include "Controllers/LyokoPlayerControllerBase.h"
#include "InputActionValue.h"
#include "LyokoPlayerController.generated.h"

//----------------------------------------------------------------------------------------------------------------------
class UInputMappingContext;
class UInputAction;

//----------------------------------------------------------------------------------------------------------------------
/**
 * @brief ALyokoPlayerController is an abstract base class for player controllers in the Lyoko game.
 * It extends ALyokoPlayerControllerBase and provides functionality for handling input devices and pause state.
 */
UCLASS(Abstract)
class LYOKOBASE_API ALyokoPlayerController : public ALyokoPlayerControllerBase
{
    GENERATED_BODY()

public:
    //------------------------------------------------------------------------------------------------------------------
    //TODO@g Move RespawnPoint to PlayerState
    FTransform RespawnPoint;

public:
    //------------------------------------------------------------------------------------------------------------------
    virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;

    //------------------------------------------------------------------------------------------------------------------
    // Input Device Changes
    //------------------------------------------------------------------------------------------------------------------
protected:
    /** 
     * @brief Called when the gamepad becomes the active input device. This function can be overridden in Blueprints or C++ to handle gamepad-specific logic.
     */
    UFUNCTION(BlueprintNativeEvent, Category = "Lyoko Player Controller|Input")
    void OnGamepadActive();
    virtual void OnGamepadActive_Implementation();

    /** 
     * @brief Called when the mouse and keyboard become the active input device. This function can be overridden in Blueprints or C++ to handle mouse and keyboard-specific logic.
     */
    UFUNCTION(BlueprintNativeEvent, Category = "Lyoko Player Controller|Input")
    void OnMouseKeyboardActive();
    virtual void OnMouseKeyboardActive_Implementation();

    //------------------------------------------------------------------------------------------------------------------
    // Pause
    //------------------------------------------------------------------------------------------------------------------
private:
    /**
    * @brief Indicates whether the game is currently paused. This variable is used to track the game's pause state and can be accessed through the IsGamePaused() function.
    */
    bool bIsGamePaused = false;

public:
    //------------------------------------------------------------------------------------------------------------------
    /** 
     * @brief System Input Mapping Context.
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|System")
    TObjectPtr<UInputMappingContext> SystemInputMapping;

    /**
     * @brief Pause input actions
     */
    UPROPERTY(EditDefaultsOnly, Category = "Input|System|Actions")
    TObjectPtr<UInputAction> PauseInputAction;

public:
    /**
     * @brief Returns whether the game is currently paused. This function can be called from Blueprints or C++ to check the game's pause state.
     * @return True if the game is paused, false otherwise.
     */
    UFUNCTION(BlueprintCallable, Category = "Lyoko Player Controller|Pause")
    bool IsGamePaused() const { return bIsGamePaused; }

protected:
    /**
     * @brief Called when the game is paused. This function can be overridden in Blueprints or C++ to handle game pause logic.
     */
    UFUNCTION(BlueprintNativeEvent, Category = "Lyoko Player Controller|Pause")
    void OnGamePaused();
    virtual void OnGamePaused_Implementation() {};

    /**
     * @brief Called when the game is resumed from a paused state. This function can be overridden in Blueprints or C++ to handle game resume logic.
     */
    UFUNCTION(BlueprintNativeEvent, Category = "Lyoko Player Controller|Pause")
    void OnGameResumed();
    virtual void OnGameResumed_Implementation() {};

public:
    /** 
     * @brief Sets the game paused state. This function can be called from Blueprints or C++ to pause or unpause the game.
     * @param bPaused Whether the game should be paused or not.
     * @return True if the game paused state was changed, false if it was already in the desired state.
     */
    UFUNCTION(BlueprintCallable, Category = "Lyoko Player Controller|Pause")
    bool SetGamePaused(bool bPaused);

private:
    /**
     * @brief Toggles the game's paused state. This function is called when the pause input action is triggered.
     * @param Value The input action value.
     */
    void TogglePause(const FInputActionValue &Value);
};
