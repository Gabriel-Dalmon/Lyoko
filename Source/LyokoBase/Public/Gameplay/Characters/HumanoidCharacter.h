// Copyright © 2025 Lyoko - 96 l'Art Cheperdu

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Characters/LyokoCharacterBase.h"
#include "Gameplay/Characters/MovementCharacter.h"
#include "Gameplay/Pickuper.h"
#include "Gameplay/Items/ItemBase.h"
#include "UObject/ScriptInterface.h"
#include "HumanoidCharacter.generated.h"

class IPickupableItem;

/**
 * 
 */
UCLASS()
class LYOKOBASE_API AHumanoidCharacter : public ALyokoCharacterBase, public IMovementCharacter, public IPickuper
{
	GENERATED_BODY()
	
protected:
	TObjectPtr<AItemBase> MainItem;

public:
	virtual void Move_Implementation(const FVector2D &Direction) override;

	virtual void Pickup_Implementation(const TScriptInterface<IPickupableItem>& Pickupable);
	
	virtual void OnPickupableInReach_Implementation(const TScriptInterface<IPickupableItem>& Pickupable);

	virtual void OnPickupableOutOfReach_Implementation(const TScriptInterface<IPickupableItem>& Pickupable);
};
