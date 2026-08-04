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
#include "DamageModifierProperty.generated.h"

//----------------------------------------------------------------------------------------------------------------------
/**
 * Property for modifying damage dealt by a weapon.
 */
UCLASS()
class LYOKOWEAPONS_API UDamageModifierProperty : public ULyokoProperty
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float DamageModifier = 1.0f;
};
