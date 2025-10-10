// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LyokoGameModeBase.generated.h"

class ALyokoPlayerControllerBase;

/**
 * 
 */
UCLASS()
class LYOKOBASE_API ALyokoGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	void Respawn(ALyokoPlayerControllerBase *Controller);
};
