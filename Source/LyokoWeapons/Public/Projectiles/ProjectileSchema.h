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
#include "Model/LyokoSchema.h"
#include "ProjectileSchema.generated.h"

//----------------------------------------------------------------------------------------------------------------------
/**
 * @brief Represents the definition of a projectile, including its damage, radius, on-hit behavior,
 * and associated properties for composition.
 *
 * This class follows the ULyokoSchema composition pattern, allowing modular data assets where
 * each projectile type can compose only the properties it needs.
 */
UCLASS(BlueprintType)
class LYOKOWEAPONS_API UProjectileSchema : public ULyokoSchema
{
    GENERATED_BODY()

public:
    /**
     * @brief The display name of the projectile schema.
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Display")
    FText DisplayName;

    /**
     * @brief The description of the projectile schema.
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Display")
    FText Description;
};
