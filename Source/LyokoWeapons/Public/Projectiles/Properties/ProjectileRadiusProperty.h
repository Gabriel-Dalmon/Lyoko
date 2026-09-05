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
#include "ProjectileRadiusProperty.generated.h"

//----------------------------------------------------------------------------------------------------------------------
/**
 * @brief Property for the collision sphere radius of a projectile.
 */
UCLASS()
class LYOKOWEAPONS_API UProjectileRadiusProperty : public ULyokoProperty
{
    GENERATED_BODY()

public:
    /**
     * @brief The radius of the projectile's collision sphere.
     */
    UPROPERTY(EditAnywhere, Category = "Projectile Radius Property")
    float Radius = 5.0f;
};
