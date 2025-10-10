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

class UInputMappingContext;
class UInputAction;

/**
 * 
 */
UCLASS()
class LYOKOPLAYER_API ATopDownPlayerController : public ALyokoPlayerControllerBase, public IOnPlayerRestartedHook
{
	GENERATED_BODY()


public:
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext *MovementMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction *MovementInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction *PauseInputAction;

	float cooldown = 0.0f;
protected:
	virtual void SetupInputComponent() override;

	// To add mapping context
	virtual void BeginPlay();

	void Move(const FInputActionValue &Value);
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



