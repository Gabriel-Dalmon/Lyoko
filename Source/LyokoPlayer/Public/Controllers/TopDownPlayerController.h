// Copyright © 2026 Lyoko - 96 l'Art Cheperdu
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

/**
 * 
 */
UCLASS(Abstract)
class LYOKOPLAYER_API ATopDownPlayerController : public ALyokoPlayerController, public IOnPlayerRestartedHook, public ICombatActor
{
    GENERATED_BODY()


public:

    /** Movement mapping context */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Movement")
    TSoftObjectPtr<class UInputMappingContext> MovementInputMapping;

    UPROPERTY(EditDefaultsOnly, Category = "Input|Movement|Actions")
    TSoftObjectPtr<class UInputAction> MovementInputAction;

    /** Movement mapping context */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Look")
    TSoftObjectPtr<class UInputMappingContext> LookInputMapping;

    UPROPERTY(EditDefaultsOnly, Category = "Input|Look|Actions")
    TSoftObjectPtr<class UInputAction> LookInputAction;

    /** Interactions mapping context */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Interactions")
    TSoftObjectPtr<class UInputMappingContext> InteractionsInputMapping;

    UPROPERTY(EditDefaultsOnly, Category = "Input|Interactions|Actions")
    TSoftObjectPtr<class UInputAction> PrimaryInteractInputAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input|Interactions|Actions")
    TSoftObjectPtr<class UInputAction> SecondaryInteractInputAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input|Interactions|Actions")
    TSoftObjectPtr<class UInputAction> TernaryInteractInputAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input|Pause")
    TSoftObjectPtr<class UInputAction> PauseInputAction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|Mouse Look")
    bool bEnableMouseLook_TO_REVIEW = true;

    UPROPERTY(BlueprintReadWrite, Category = "Input|Mouse Look")
    bool bLookAtCursor = true;

protected:

    virtual void SetupInputComponent() override;

    // To add mapping context
    virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Top Down Controller Callbacks")
    void OnMove(const FInputActionValue &Value);
    virtual void OnMove_Implementation(const FInputActionValue &Value);

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Top Down Controller Callbacks")
    void OnLook(const FInputActionValue &Value);
    virtual void OnLook_Implementation(const FInputActionValue &Value);

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Top Down Controller Callbacks")
    void OnPrimaryInteract(const FInputActionValue& Value);
    virtual void OnPrimaryInteract_Implementation(const FInputActionValue& Value);

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Top Down Controller Callbacks")
    void OnSecondaryInteract(const FInputActionValue& Value);
    virtual void OnSecondaryInteract_Implementation(const FInputActionValue& Value);

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Top Down Controller Callbacks")
    void OnTernaryInteract(const FInputActionValue& Value);
    virtual void OnTernaryInteract_Implementation(const FInputActionValue& Value);

protected:
    void ForwardInteractionToPawn(EInteractionTypes Type) const;

public:
    void LookAtCursor();
    void SetControlRotationToCamera(const APawn &NewPawn);

protected:
    virtual void OnGamepadActive_Implementation() override;
    virtual void OnMouseKeyboardActive_Implementation() override;

public:
    inline virtual FVector GetAttackDirection_Implementation(FVector AttackOrigin) const
    {
        FVector Direction;
        if (bLookAtCursor) {
            FVector RayStart, RayDirection;
            DeprojectMousePositionToWorld(RayStart, RayDirection);
            FVector RayHitLocation = FMath::LinePlaneIntersection(RayStart, RayStart + RayDirection * 10000.0f, FVector(0.0f, 0.0f, AttackOrigin.Z), FVector::UpVector);
            Direction = RayHitLocation - AttackOrigin;
        }
        else if (APawn *PossessedPawn = GetPawn(); PossessedPawn) {
            Direction = PossessedPawn->GetActorRotation().Vector();
        }

        Direction.Z = 0.0f;
        return Direction.GetSafeNormal();
    }

    UFUNCTION(BlueprintImplementableEvent)
    void PauseGame();

    virtual void OnPossess(APawn *InPawn) override;
    virtual void OnUnPossess() override;

    virtual void OnPlayerRestarted_Implementation() override;
    
    UFUNCTION()
    virtual void OnPossessedPawnDead();

    virtual void Tick(float DeltaTime) override;
};



