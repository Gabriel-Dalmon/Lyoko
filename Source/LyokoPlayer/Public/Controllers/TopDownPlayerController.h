// Copyright © 2025 Lyoko - 96 l'Art Cheperdu

#pragma once

#include "CoreMinimal.h"
#include "Controllers/LyokoPlayerControllerBase.h"
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
class LYOKOPLAYER_API ATopDownPlayerController : public ALyokoPlayerControllerBase, public IOnPlayerRestartedHook, public ICombatActor
{
	GENERATED_BODY()


public:

	/** Movement mapping context */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Movement")
	TSoftObjectPtr<class UInputMappingContext> MovementInputMapping;

	UPROPERTY(EditDefaultsOnly, Category = "Input|Movement|Actions")
	TSoftObjectPtr<class UInputAction> MovementInputAction;

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

protected:

	virtual void SetupInputComponent() override;

	// To add mapping context
	virtual void BeginPlay();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Top Down Controller Callbacks")
	void OnMove(const FInputActionValue &Value);
	virtual void OnMove_Implementation(const FInputActionValue &Value);

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

public:
	inline virtual FVector GetAttackDirection_Implementation(FVector AttackOrigin) const
	{
		FVector RayStart, RayDirection;
		DeprojectMousePositionToWorld(RayStart, RayDirection);
		FVector RayHitLocation = FMath::LinePlaneIntersection(RayStart, RayStart + RayDirection * 10000.0f, FVector(0.0f, 0.0f, AttackOrigin.Z), FVector::UpVector);

		FVector Direction = RayHitLocation - AttackOrigin;
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



