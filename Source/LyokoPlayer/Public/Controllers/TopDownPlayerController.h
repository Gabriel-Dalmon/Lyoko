// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Controllers/LyokoPlayerControllerBase.h"
#include "EnhancedInputComponent.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h" 
#include "Core/HooksInterfaces/OnPlayerRestartedHook.h"
#include "TopDownPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class LYOKOPLAYER_API ATopDownPlayerController : public ALyokoPlayerControllerBase, public IOnPlayerRestartedHook
{
	GENERATED_BODY()


public:

	/** Movement mapping context */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Movement")
	TSoftObjectPtr<class UInputMappingContext> MovementInputMapping;

	UPROPERTY(EditDefaultsOnly, Category = "Input|Movement|Actions")
	TSoftObjectPtr<class UInputAction> MovementInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input|Pause")
	TSoftObjectPtr<class UInputAction> PauseInputAction;

protected:

	virtual void SetupInputComponent() override;

	// To add mapping context
	virtual void BeginPlay();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Top Down Controller Callbacks")
	void OnMove(const FInputActionValue &Value);
	virtual void OnMove_Implementation(const FInputActionValue &Value);

	void LookAtCursor();
	void SetControlRotationToCamera(const APawn &NewPawn);

	UFUNCTION(BlueprintImplementableEvent)
	void PauseGame();

	virtual void OnPossess(APawn *InPawn) override;
	virtual void OnUnPossess() override;

	virtual void OnPlayerRestarted_Implementation() override;
	
	UFUNCTION()
	virtual void OnPossessedPawnDead();

	virtual void Tick(float DeltaTime) override;
};



