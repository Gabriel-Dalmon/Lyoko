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
#include "Items/WeaponBase.h"
#include "Projectiles/ProjectileBase.h"
#include "Items/Properties/ReloadProperty.h"
#include "Items/Properties/ProjectileProperty.h"
#include "Items/Properties/DamageModifierProperty.h"
#include "RangedWeapon.generated.h"

//----------------------------------------------------------------------------------------------------------------------
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnReloaded, int, BatchSize);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAmmunitionEmpty);

//----------------------------------------------------------------------------------------------------------------------
/**
 * 
 */
UCLASS(Abstract)
class LYOKOWEAPONS_API ARangedWeapon : public AWeaponBase
{
    GENERATED_BODY()

protected:
    UPROPERTY(BlueprintReadWrite)
    int CurrentAmmunitionCount = 0;

public:
    inline virtual TSet<TSubclassOf<ULyokoProperty>> GetMandatoryProperties_Implementation() const override {
        auto Properties = Super::GetMandatoryProperties_Implementation();
        Properties.Append({ UReloadProperty::StaticClass(), UProjectileProperty::StaticClass(), UDamageModifierProperty::StaticClass() });
        return Properties;
    }

    //------------------------------------------------------------------------------------------------------------------
    // WeaponBase Overrides
    //------------------------------------------------------------------------------------------------------------------
public:
    virtual void OnSecondaryInteracted_Implementation() override;
    virtual void AttackInDirection_Implementation(const FVector& Direction) override;
    virtual bool IsAttackAvailable_Implementation() const override;
    virtual FVector GetDefaultAttackDirection_Implementation() const override;

protected:
    virtual float ComputeDamageMultiplier_Implementation() const override;

public:
    UPROPERTY(BlueprintAssignable, Category = "Gameplay|Weapon|Ranged")
    FOnAmmunitionEmpty OnAmmunitionEmptyEvent;

protected:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay|Weapon|Ranged")
    void OnAmmunitionEmpty();
    virtual void OnAmmunitionEmpty_Implementation();

#pragma region Fire
    //------------------------------------------------------------------------------------------------------------------
    // Fire
    //------------------------------------------------------------------------------------------------------------------
public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay|Weapon|Ranged")
    bool IsFireAvailable() const;
    virtual bool IsFireAvailable_Implementation() const;

    void Fire(const FVector &OffsetFromMuzzle, const FVector &Direction);

    void Fire(const FVector &OffsetFromMuzzle, const FVector &Direction, const float InitialSpeed);

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Gameplay|Weapon|Ranged")
    void FireInDirection(const FVector &Direction);
    void FireInDirection_Implementation(const FVector &Direction);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay|Weapon|Ranged")
    void OnFired(AProjectileBase *Projectile);
    virtual void OnFired_Implementation(AProjectileBase *Projectile);


#pragma endregion Fire

#pragma region Reload
    //------------------------------------------------------------------------------------------------------------------
    // Reload
    //------------------------------------------------------------------------------------------------------------------
public:
    /**
     * @brief If true, the weapon will automatically reload when ammunition is empty. If false, the player must manually reload.
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay|Weapon|Ranged")
    bool bAutomaticReload = true;

    /**
     * @brief Event triggered when the weapon has finished reloading.
     * @param BatchSize The number of ammunition units reloaded.
     */
    UPROPERTY(BlueprintAssignable, Category = "Gameplay|Weapon|Ranged")
    FOnReloaded OnReloadedEvent;

private:
    /**
     * @brief Timer handle for managing the reload process. This is used to schedule the completion of the reload after a specified duration.
     */
    FTimerHandle ReloadTimerHandle;

public:
    /**
     * @brief Checks if the weapon is currently in the process of reloading.
     */
    UFUNCTION(BlueprintCallable, Category = "Gameplay|Weapon|Ranged")
    bool IsReloading() const;

    /**
     * @brief Checks if the weapon is available for reloading based on its current state and any relevant conditions.
     */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay|Weapon|Ranged")
    bool IsReloadAvailable() const;
    virtual bool IsReloadAvailable_Implementation() const;

    /**
     * @brief Initiates the reload process for the weapon using the reload pattern defined in the weapon's data or overriden by the owner.
     */
    UFUNCTION(BlueprintCallable, Category = "Gameplay|Weapon|Ranged")
    void Reload();

    /**
     * @brief Reloads the weapon to its maximum ammunition capacity. This is typically used for weapons that reload all at once.
     */
    UFUNCTION(BlueprintCallable, Category = "Gameplay|Weapon|Ranged")
    void ReloadMax();

    /**
     * @brief Reloads the weapon by a specified batch size. This is typically used for weapons that reload in increments.
     * @param BatchSize The number of ammunition units to reload in this batch.
     */
    UFUNCTION(BlueprintCallable, Category = "Gameplay|Weapon|Ranged")
    void ReloadBatch(int BatchSize);

    /**
     * @brief Cancels the ongoing reload process, if any. This can be used to interrupt a reload action, for example, if the player decides to fire the weapon before the reload is complete.
     */
    UFUNCTION(BlueprintCallable, Category = "Gameplay|Weapon|Ranged")
    void CancelReload();

protected:
    /**
     * @brief Called when the reload process starts. This can be used to trigger animations, sounds, or other effects associated with reloading.
     */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay|Weapon|Ranged")
    void OnReloadStarted();
    virtual void OnReloadStarted_Implementation();

    /**
     * @brief Called when the reload process completes successfully. This can be used to update the ammunition count, trigger animations, sounds, or other effects associated with completing a reload.
     * @param BatchSize The number of ammunition units reloaded in this batch.
     */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay|Weapon|Ranged")
    void OnReloaded(int BatchSize);
    virtual void OnReloaded_Implementation(int BatchSize);

    /**
     * @brief Called when the reload process is canceled before completion. This can be used to revert any changes made during the reload process, such as stopping animations or sounds.
     */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay|Weapon|Ranged")
    void OnReloadCanceled();
    virtual void OnReloadCanceled_Implementation();
#pragma endregion Reload

protected:
    /* Name of the Muzzle Socket */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sockets", meta = (EditCondition = "bEnableSocketNamesOverride"))
    FName MuzzleSocketName = TEXT("Muzzle");
public:
    FTransform GetMuzzleTransform(ERelativeTransformSpace TransformSpace = RTS_World) const;
    float GetProjectileDefaultInitialSpeed() const;
    float GetProjectileRadius() const;

    inline virtual FVector GetAttackOrigin_Implementation() const override
    {
        return GetMuzzleTransform(ERelativeTransformSpace::RTS_World).GetLocation();
    }
};
