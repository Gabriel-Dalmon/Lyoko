// Copyright © 2025 Lyoko - 96 l'Art Cheperdu

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UObject/ScriptInterface.h"
#include "PickupableItem.generated.h"

class IPickuper;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPickupableItem : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class LYOKOBASE_API IPickupableItem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Pickuper")
	void OnPickup(const TScriptInterface<IPickuper>& Pickuper);
};
