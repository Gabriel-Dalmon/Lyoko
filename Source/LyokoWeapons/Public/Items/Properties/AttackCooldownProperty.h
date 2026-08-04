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
#include "Model/LyokoProperty.h"
#include "AttackCooldownProperty.generated.h"

//----------------------------------------------------------------------------------------------------------------------
/**
 * Property for modifying the attack cooldown of a weapon.
 */
UCLASS()
class LYOKOWEAPONS_API UAttackCooldownProperty : public ULyokoProperty
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float AttackCooldown = 1.0f;
};
