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
#include "UObject/Object.h"
#include "ItemProperty.generated.h"

//----------------------------------------------------------------------------------------------------------------------
/**
 * @brief Base class for item properties in the game.
 */
UCLASS(Abstract, BlueprintType, Blueprintable, EditInlineNew, DefaultToInstanced)
class LYOKOBASE_API UItemProperty : public UObject
{
    GENERATED_BODY()

};
