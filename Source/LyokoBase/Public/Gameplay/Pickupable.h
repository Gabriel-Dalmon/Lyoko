// Copyright � 2025 Lyoko - 96 l'Art Cheperdu

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

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Pickupable")
    TScriptInterface<IPickuper> GetCarrier() const;

    
#pragma region METHODS_WITH_DEFAULT_IMPLEMENTATIONS
public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Pickupable")
    bool IsCarried() const;
    virtual bool IsCarried_Implementation() const;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Pickupable")
    bool CanBeStolen(const TScriptInterface<IPickuper>& Stealer) const;
    virtual bool CanBeStolen_Implementation(const TScriptInterface<IPickuper>& Stealer) const;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Pickupable")
    void OnStolen(const TScriptInterface<IPickuper>& Stealer);
    virtual void OnStolen_Implementation(const TScriptInterface<IPickuper>& Stealer);
#pragma endregion METHODS_WITH_DEFAULT_IMPLEMENTATIONS
};
