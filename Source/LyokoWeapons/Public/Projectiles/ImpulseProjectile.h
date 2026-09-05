// Copyright © 2025-2027 Lyoko - 96 l'Art Cheperdu
//     __                __       
//    / /   __  ______  / /______ 
//   / /   / / / / __ \/ //_/ __ \
//  /___/ /_/ / /_/ / ,< / /_/ /
// /_____/ \__, / \____/_/|_| \____/ 
//       /____/                    

#pragma once

//----------------------------------------------------------------------------------------------------------------------
#include "CoreMinimal.h"
#include "Projectiles/ProjectileBase.h"
#include "Projectiles/Properties/ProjectileImpulseProperty.h"
#include "ImpulseProjectile.generated.h"

//----------------------------------------------------------------------------------------------------------------------
/**
 * @brief Projectile that applies impulse force on hit to physics-enabled components
 * and character movement components.
 *
 * This projectile type requires a UProjectileSchema with a UProjectileImpulseProperty
 * composed into it. The impulse strength determines the force applied, and the
 * MasterFieldClass (if set) spawns an actor at the hit location to destroy
 * Geometry Collections.
 */
UCLASS()
class LYOKOWEAPONS_API AImpulseProjectile : public AProjectileBase
{
    GENERATED_BODY()
    
public:
    /**
     * @brief Applies impulse force to hit components based on the impulse property data.
     */
    virtual void OnHit_Implementation(UPrimitiveComponent* HitComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

protected:
    /**
     * @brief Applies impulse to the hit components.
     * @param HitComp The component that was hit.
     * @param OtherComp The component of the hit actor.
     * @param Hit The hit result details.
     * @param OtherActor The actor that was hit.
     */
    void ApplyImpulse(UPrimitiveComponent* HitComp, UPrimitiveComponent* OtherComp, const FHitResult& Hit, AActor* OtherActor);

public:
    /**
     * @brief Gets the impulse property from the projectile schema.
     * @return Pointer to the impulse property, or nullptr if not found.
     */
    inline UProjectileImpulseProperty *GetImpulseProperty() const
    {
        return GetProperty<UProjectileImpulseProperty>();
    }

    /**
     * @brief Gets the impulse strength from the projectile schema.
     * @return The impulse strength value.
     */
    UFUNCTION(BlueprintCallable, Category = "Gameplay|Weapon|Impulse")
    float GetImpulseStrength() const;

    /**
     * @brief Gets the master field class from the projectile schema.
     * @return The master field actor class, or nullptr if not set.
     */
    UFUNCTION(BlueprintCallable, Category = "Gameplay|Weapon|Impulse")
    TSubclassOf<AActor> GetMasterFieldClass() const;
};
