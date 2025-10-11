// Copyright © 2025 Lyoko - 96 l'Art Cheperdu

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Characters/LyokoCharacterBase.h"
#include "Gameplay/Characters/MovementCharacter.h"
#include "HumanoidCharacter.generated.h"

/**
 * 
 */
UCLASS()
class LYOKOBASE_API AHumanoidCharacter : public ALyokoCharacterBase, public IMovementCharacter
{
	GENERATED_BODY()
	
public:
	virtual void Move_Implementation(const FVector2D &Direction) override;
};
