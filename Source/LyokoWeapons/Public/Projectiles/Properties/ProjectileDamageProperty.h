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
#include "ProjectileDamageProperty.generated.h"

//----------------------------------------------------------------------------------------------------------------------
/**
 * @brief Property for the damage value dealt by a projectile on hit.
 */
UCLASS()
class LYOKOWEAPONS_API UProjectileDamageProperty : public ULyokoProperty
{
    GENERATED_BODY()

public:
    /**
     * @brief The base damage value dealt by the projectile.
     */
    UPROPERTY(EditAnywhere, Category = "Projectile Damage Property")
    float Damage = 10.0f;
};
