// Copyright � 2025-2027 Lyoko - 96 l'Art Cheperdu
//     __                __       
//    / /   __  ______  / /______ 
//   / /   / / / / __ \/ //_/ __ \
//  / /___/ /_/ / /_/ / ,< / /_/ /
// /_____/\__, /\____/_/|_|\____/ 
//       /____/                   

#pragma once

#include "CoreMinimal.h"
#include "Controllers/LyokoPlayerController.h"
#include "EnhancedInputComponent.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h" 
#include "Gameplay/InteractionTypes.h"
#include "Gameplay/Combat/CombatActor.h"
#include "Core/HooksInterfaces/OnPlayerRestartedHook.h"
#include "TopDownPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

//----------------------------------------------------------------------------------------------------------------------
/**
 * @brief A player controller class for top-down games, handling input and interactions with the possessed pawn.
 */
UCLASS(Abstract)
class LYOKOPLAYER_API ATopDownPlayerController : public ALyokoPlayerController, public IOnPlayerRestartedHook, public ICombatActor
{
    GENERATED_BODY()

public:
    virtual void SetupInputComponent() override;
    virtual void OnPossess(APawn *InPawn) override;
    virtual void OnUnPossess() override;
    virtual void Tick(float DeltaTime) override;

    /**
     * @brief Called by LyokoGameModeBase at the end of the RestartPlayer method.
     * The possessed pawn is expected to use the Control Rotation.
     * It is computed based on the camera direction to ensure topdown movement to be aligned with the view.
     * When the pawn gets possessed the control rotation is overridden by Unreal internal logic in some flows. 
     * This method is used as hook when Control Rotation is no longer overriden by Unreal internal logic.
     */
    virtual void OnPlayerRestarted_Implementation() override;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Top Down Controller Callbacks")
    void OnPossessedPawnDead();
    virtual void OnPossessedPawnDead_Implementation();

protected:
    //------------------------------------------------------------------------------------------------------------------
    // Movement
    //------------------------------------------------------------------------------------------------------------------
#pragma region Movement
protected:
    /** 
     * @brief Movement mapping context
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Movement")
    TObjectPtr<UInputMappingContext> MovementInputMapping;

    /**
     * @brief Movement input action
     */
    UPROPERTY(EditDefaultsOnly, Category = "Input|Movement|Actions")
    TObjectPtr<UInputAction> MovementInputAction;

    /**
     * @brief Dash input action
     */
    UPROPERTY(EditDefaultsOnly, Category = "Input|Movement|Actions")
    TObjectPtr<UInputAction> DashInputAction;

protected:
    /**
     * @brief Notifies the possessed pawn a movement has been requested.
     * @param Value The input value representing the movement direction and magnitude.
     */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Top Down Controller Callbacks")
    void OnMove(const FInputActionValue &Value);
    virtual void OnMove_Implementation(const FInputActionValue &Value);

    /**
     * @brief Notifies the possessed pawn a dash has been requested.
     * @param Value The input value representing the dash type and magnitude.
     */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Top Down Controller Callbacks")
    void OnDash(const FInputActionValue &Value);
    virtual void OnDash_Implementation(const FInputActionValue &Value);

    /**
     * @brief Sets the control rotation of the player controller to match the camera's orientation from a topdown perspective.
     */
    void SetControlRotationToCamera(const APawn &NewPawn);
#pragma endregion Movement

protected:
    //------------------------------------------------------------------------------------------------------------------
    // Look
    //------------------------------------------------------------------------------------------------------------------
#pragma region Look
protected:
    /** 
     * @brief Look mapping context
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Look")
    TObjectPtr<UInputMappingContext> LookInputMapping;

    /**
     * @brief Look input action
     */
    UPROPERTY(EditDefaultsOnly, Category = "Input|Look|Actions")
    TObjectPtr<UInputAction> LookInputAction;

protected:
    /**
     * @brief Called when the player provides input to look around. Forwards the new look input to the possessed pawn.
     * @param Value The input value representing the look direction and magnitude.
     */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Top Down Controller Callbacks")
    void OnLook(const FInputActionValue &Value);
    virtual void OnLook_Implementation(const FInputActionValue &Value);

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|Mouse Look")
    bool bEnableMouseLook_TO_REVIEW = true;

private:
    /**
     * @brief A boolean flag indicating wether the player character look logic should be based on the mouse cursor position.
     */
    bool bLookAtCursor = true;

    /**
     * @brief Compute the look direction based on the mouse cursor position and forwards the new look input to the possessed pawn.
     */
    void LookAtCursor();
#pragma endregion Look

protected:
    //------------------------------------------------------------------------------------------------------------------
    // Interactions
    //------------------------------------------------------------------------------------------------------------------
#pragma region Interactions
protected:
    /**
     * @brief Interactions mapping context
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Interactions")
    TObjectPtr<UInputMappingContext> InteractionsInputMapping;

    /**
     * @brief Primary interaction input action
     */
    UPROPERTY(EditDefaultsOnly, Category = "Input|Interactions|Actions")
    TObjectPtr<UInputAction> PrimaryInteractInputAction;

    /**
     * @brief Secondary interaction input action
     */
    UPROPERTY(EditDefaultsOnly, Category = "Input|Interactions|Actions")
    TObjectPtr<UInputAction> SecondaryInteractInputAction;

    /**
     * @brief Ternary interaction input action
     */
    UPROPERTY(EditDefaultsOnly, Category = "Input|Interactions|Actions")
    TObjectPtr<UInputAction> TernaryInteractInputAction;

protected:
    /**
     * @brief Notifies the possessed pawn a primary interaction has been requested.
     * @param Value The input value representing the interaction type and magnitude.
     */
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Top Down Controller Callbacks")
    void OnPrimaryInteract(const FInputActionValue &Value);
    virtual void OnPrimaryInteract_Implementation(const FInputActionValue &Value);

    /**
     * @brief Notifies the possessed pawn a secondary interaction has been requested.
     * @param Value The input value representing the interaction type and magnitude.
     */
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Top Down Controller Callbacks")
    void OnSecondaryInteract(const FInputActionValue &Value);
    virtual void OnSecondaryInteract_Implementation(const FInputActionValue &Value);

    /**
     * @brief Notifies the possessed pawn a ternary interaction has been requested.
     * @param Value The input value representing the interaction type and magnitude.
     */
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Top Down Controller Callbacks")
    void OnTernaryInteract(const FInputActionValue &Value);
    virtual void OnTernaryInteract_Implementation(const FInputActionValue &Value);

    /**
     * @brief Helper function to forward the interaction request to the possessed pawn.
     * @param Type The type of interaction requested (Primary, Secondary, or Ternary).
     */
    void ForwardInteractionToPawn(EInteractionTypes Type) const;
#pragma endregion Interactions

protected:
    //------------------------------------------------------------------------------------------------------------------
    // Combat
    //------------------------------------------------------------------------------------------------------------------
#pragma region Combat
    protected:
        virtual FVector GetAttackDirection_Implementation(FVector AttackOrigin) const override;

#pragma endregion Combat

protected:
    //------------------------------------------------------------------------------------------------------------------
    // Pause
    //------------------------------------------------------------------------------------------------------------------
#pragma region Pause
protected:
    virtual void OnGamePaused_Implementation() override;
    virtual void OnGameResumed_Implementation() override;
#pragma endregion Pause
};



