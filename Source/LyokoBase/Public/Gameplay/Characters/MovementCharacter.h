// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MovementCharacter.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMovementCharacter : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class LYOKOBASE_API IMovementCharacter
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Movement Character")
	void Move(const FVector2D &Direction);
};
