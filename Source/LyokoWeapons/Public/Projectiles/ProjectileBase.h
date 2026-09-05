// Copyright © 2025-2027 Lyoko - 96 l'Art Cheperdu
//     __                __       
//    / /   __  ______  / /______ 
//   / /   / / / / __ \/ //_/ __ \\
//  / /___/ /_/ / /_/ / ,< / /_/ /
// /_____/\__, /\____/_/|_|\____/ 
//       /____/                   

#pragma once

//----------------------------------------------------------------------------------------------------------------------
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Projectiles/ProjectileSchema.h"
#include "ProjectileBase.generated.h"

//----------------------------------------------------------------------------------------------------------------------
/**
 * @brief Base class for projectile actors that uses the ULyokoSchema composition system.
 *
 * Each projectile type composes its data via properties (UProjectileDamageProperty,
 * UProjectileRadiusProperty, UProjectileOnHitBehaviorProperty, etc.) stored in a
 * UProjectileSchema data asset. This provides a modular, single-source-of-truth approach
 * where each projectile schema contains only the properties it needs.
 *
 * Schema implementation pattern (the only necessary elements for a schema-backed actor):
 *   1. IsXSchemaValid() — validates the schema reference
 *   2. GetMandatoryProperties() — overridable in C++ and Blueprint
 *   3. GetOptionalProperties() — overridable in C++ and Blueprint
 *   4. A wrapper to get properties (GetProperty<T>())
 *   5. A wrapper to get the Schema (GetProjectileSchema())
 *
 * Note: PostEditChangeProperty and GetMinimumProjectileSchemaClass are NOT necessary for the schema system.
 * They were remnants of the old inheritance-based approach and have been removed.
 */
UCLASS()
class LYOKOWEAPONS_API AProjectileBase : public AActor
{
    GENERATED_BODY()

public:
    /* Sphere collision component */
    UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
    USphereComponent *CollisionComp;

    /* Mesh Component */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UStaticMeshComponent> MeshComponent;

    /* Projectile movement component */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
    UProjectileMovementComponent *ProjectileMovement;

private:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UProjectileSchema> ProjectileSchema;

public:
    /**
     * @brief Gets the projectile schema data asset.
     */
    UFUNCTION(BlueprintCallable, Category = "Gameplay|Weapon")
    inline UProjectileSchema *GetProjectileSchema() const
    {
        return ProjectileSchema;
    }

    /**
     * @brief Gets a property of the specified type from the projectile schema.
     * @tparam PropertyType The type of property to retrieve (must extend ULyokoProperty).
     * @return Pointer to the property, or nullptr if not found.
     */
    template<typename PropertyType>
    inline PropertyType *GetProperty() const
    {
        static_assert(TIsDerivedFrom<PropertyType, ULyokoProperty>::IsDerived, "PropertyType must be derived from ULyokoProperty");
        return ProjectileSchema ? ProjectileSchema->GetProperty<PropertyType>() : nullptr;
    }

    /**
     * @brief Gets the damage value from the projectile schema's damage property.
     * @return The base damage value.
     */
    UFUNCTION(BlueprintCallable, Category = "Gameplay|Weapon")
    float GetDamage() const;

    /**
     * @brief Gets the collision radius from the projectile schema's radius property.
     * @return The collision sphere radius.
     */
    UFUNCTION(BlueprintCallable, Category = "Gameplay|Weapon")
    float GetRadius() const;

    /**
     * @brief Gets the on-hit behavior from the projectile schema's on-hit behavior property.
     * @return The on-hit behavior enum value.
     */
    UFUNCTION(BlueprintCallable, Category = "Gameplay|Weapon")
    EProjectileOnHitBehavior GetOnHitBehavior() const;

    /**
     * @brief Validates that the ProjectileSchema is valid (non-null) and contains
     * all mandatory properties as defined by GetMandatoryProperties().
     * @return True if the schema is valid and all mandatory properties are present.
     */
    UFUNCTION(BlueprintCallable, Category = "Gameplay|Weapon")
    bool IsProjectileSchemaValid() const;

    /**
     * @brief Returns the mandatory properties required by this projectile type.
     * Override in C++ or Blueprint to specify required properties.
     */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay|Weapon")
    TSet<TSubclassOf<ULyokoProperty>> GetMandatoryProperties() const;
    inline virtual TSet<TSubclassOf<ULyokoProperty>> GetMandatoryProperties_Implementation() const
    {
        return {};
    }

    /**
     * @brief Returns the optional properties for this projectile type.
     * Override in C++ or Blueprint to specify optional properties.
     */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay|Weapon")
    TSet<TSubclassOf<ULyokoProperty>> GetOptionalProperties() const;
    inline virtual TSet<TSubclassOf<ULyokoProperty>> GetOptionalProperties_Implementation() const
    {
        return {};
    }

public:
    /**
     * @brief Multiplier applied to the projectile's damage for this specific instance.
     */
    UPROPERTY(BlueprintReadOnly, Category = "Projectile")
    float BakedDamageMultiplier = 1.0f;

public:
    AProjectileBase();

    virtual void BeginPlay() override;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Collision")
    void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

    virtual void OnHit_Implementation(UPrimitiveComponent* HitComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Collision")
    void OnValidTargetHit(UPrimitiveComponent *HitComponent, AActor *OtherActor,
        UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit);

    virtual void OnValidTargetHit_Implementation(UPrimitiveComponent *HitComponent, AActor *OtherActor,
        UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit) {};

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Collision")
    void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

    virtual void OnBeginOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

    virtual void ApplyDamages(AActor* OtherActor);

public:
    /* Returns CollisionComp subobject */
    FORCEINLINE USphereComponent *GetCollisionComp() const { return CollisionComp; }
    /* Returns ProjectileMovement subobject */
    FORCEINLINE UProjectileMovementComponent *GetProjectileMovement() const { return ProjectileMovement; }
};
