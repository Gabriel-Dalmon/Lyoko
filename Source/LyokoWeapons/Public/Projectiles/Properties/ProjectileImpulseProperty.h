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
#include "ProjectileImpulseProperty.generated.h"

//----------------------------------------------------------------------------------------------------------------------
/**
 * @brief Property for impulse-based projectile behavior on hit.
 * Applied to physics-enabled components and character movement components.
 */
UCLASS()
class LYOKOWEAPONS_API UProjectileImpulseProperty : public ULyokoProperty
{
    GENERATED_BODY()

public:
    /**
     * @brief The strength of the impulse applied on hit.
     */
    UPROPERTY(EditAnywhere, Category = "Projectile Impulse Property")
    float ImpulseStrength = 100.0f;

    /**
     * @brief The class of the Master Field actor to spawn on hit (e.g., for destroying Geometry Collections).
     */
    UPROPERTY(EditAnywhere, Category = "Projectile Impulse Property")
    TSubclassOf<AActor> MasterFieldClass;
};
