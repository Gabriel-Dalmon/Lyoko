// Copyright © 2025 Lyoko - 96 l'Art Cheperdu

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LyokoPlayerControllerBase.generated.h"

/**
 *
 */
UCLASS()
class LYOKOBASE_API ALyokoPlayerControllerBase : public APlayerController
{
	GENERATED_BODY()

public:
	//To move to PlayerState
	FTransform RespawnPoint;

protected:
	void BindInputMapping(TSoftObjectPtr<class UInputMappingContext> InputMapping);
};
