// Copyright © 2025 Lyoko - 96 l'Art Cheperdu

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "OnPlayerRestartedHook.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UOnPlayerRestartedHook : public UInterface
{
	GENERATED_BODY()
};

/**
 * OnPlayerRestarted is called at the end of the ALyokoGameModeBase::RestartPlayer(), 
 * in ALyokoGameModeBase::FinishRestartPlayer.
 */
class LYOKOBASE_API IOnPlayerRestartedHook
{
	GENERATED_BODY()

public:
	/** Called when the player has finished restarting. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Player Restart")
	void OnPlayerRestarted();
};
