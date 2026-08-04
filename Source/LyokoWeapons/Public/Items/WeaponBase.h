// Copyright © 2025-2027 Lyoko - 96 l'Art Cheperdu
//     __                __       
//    / /   __  ______  / /______ 
//   / /   / / / / __ \/ //_/ __ \
//  / /___/ /_/ / /_/ / ,< / /_/ /
// /_____/\__, /\____/_/|_|\____/ 
//       /____/                   

#pragma once

//----------------------------------------------------------------------------------------------------------------------
#include "CoreMinimal.h"
#include "Gameplay/Items/Item.h"
#include "Gameplay/Interactable.h"
#include "Items/Properties/AttackCooldownProperty.h"
#include "WeaponBase.generated.h"

//----------------------------------------------------------------------------------------------------------------------
/**
 * 
 */
UCLASS(Abstract)
class LYOKOWEAPONS_API AWeaponBase : public AItem, public IInteractable
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite)
    float LastAttackTime = 0.0f;

public:
    inline virtual TSet<TSubclassOf<ULyokoProperty>> GetMandatoryProperties_Implementation() const override {
        auto Properties = Super::GetMandatoryProperties_Implementation();
        Properties.Append({ UAttackCooldownProperty::StaticClass() });
        return Properties;
    }

public:
    virtual void Interact_Implementation(EInteractionTypes Type) override;
    
    UFUNCTION(BlueprintNativeEvent, Category = "Gameplay|Weapon")
    void OnPrimaryInteracted();
    virtual void OnPrimaryInteracted_Implementation();

    UFUNCTION(BlueprintNativeEvent, Category = "Gameplay|Weapon")
    void OnSecondaryInteracted();
    virtual void OnSecondaryInteracted_Implementation() {};

    void Attack();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay|Weapon")
    void AttackInDirection(const FVector& Direction);
    virtual void AttackInDirection_Implementation(const FVector& Direction);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay|Weapon")
    bool IsAttackAvailable() const;
    virtual bool IsAttackAvailable_Implementation() const;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay|Weapon")
    FVector GetAttackOrigin() const;
    inline virtual FVector GetAttackOrigin_Implementation() const
    {
        return GetHandleTransform(ERelativeTransformSpace::RTS_World).GetLocation();
    }

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay|Weapon")
    FVector GetDefaultAttackDirection() const;
    virtual FVector GetDefaultAttackDirection_Implementation() const;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay|Weapon")
    float ComputeDamageMultiplier() const;
    virtual float ComputeDamageMultiplier_Implementation() const;

    UFUNCTION(BlueprintCallable, Category = "Gameplay|Weapon")
    float GetInstigatorDamageMultiplier() const;
};
