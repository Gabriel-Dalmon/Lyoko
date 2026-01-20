// Copyright © 2025 Lyoko - 96 l'Art Cheperdu

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UObject/ScriptInterface.h"
#include "Pickupable.generated.h"

class IPickuper;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPickupable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class LYOKOBASE_API IPickupable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Pickupable")
	void OnPickedUp(const TScriptInterface<IPickuper>& Pickuper);
};
