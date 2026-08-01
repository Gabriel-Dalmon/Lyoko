// Copyright © 2025 Lyoko - 96 l'Art Cheperdu

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LyokoGameModeBase.generated.h"

class ALyokoPlayerController;

/**
 * 
 */
UCLASS()
class LYOKOBASE_API ALyokoGameModeBase : public AGameModeBase
{
    GENERATED_BODY()
    
public:
    void Respawn(ALyokoPlayerController *Controller);

protected:
    virtual void FinishRestartPlayer(AController *NewPlayer, const FRotator &StartRotation) override;

};
