// Copyright © 2025-2027 Lyoko - 96 l'Art Cheperdu
//     __                __       
//    / /   __  ______  / /______ 
//   / /   / / / / __ \/ //_/ __ \
//  /___/ /_/ / /_/ / ,< / /_/ /
// /_____/\__, /\____/_/|_|\____/ 
//       /____/                   

#pragma once

//----------------------------------------------------------------------------------------------------------------------
#include "CoreMinimal.h"
#include "Model/LyokoProperty.h"
#include "Projectiles/ProjectileOnHitBehavior.h"
#include "ProjectileOnHitBehaviorProperty.generated.h"

//----------------------------------------------------------------------------------------------------------------------
/**
 * @brief Property for the behavior when a projectile hits a target.
 */
UCLASS()
class LYOKOWEAPONS_API UProjectileOnHitBehaviorProperty : public ULyokoProperty
{
    GENERATED_BODY()

public:
    /**
     * @brief The behavior to execute when the projectile hits a valid target.
     */
    UPROPERTY(EditAnywhere, Category = "Projectile OnHit Behavior Property")
    EProjectileOnHitBehavior OnHitBehavior = EProjectileOnHitBehavior::Destroy;
};
