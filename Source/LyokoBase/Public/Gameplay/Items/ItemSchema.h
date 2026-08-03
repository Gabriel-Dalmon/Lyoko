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
#include "Model/LyokoSchema.h"
#include "ItemSchema.generated.h"

//----------------------------------------------------------------------------------------------------------------------
/**
 * @brief Represents the definition of an item, including its display name, description, icon, and associated properties.
 */
UCLASS(BlueprintType)
class LYOKOBASE_API UItemSchema : public ULyokoSchema
{
    GENERATED_BODY()
    
public:
    /**
     * @brief The display name of the item.
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FText DisplayName;

    /**
     * @brief The description of the item.
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FText Description;

    /**
     * @brief The icon representing the item.
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TObjectPtr<UTexture2D> Icon;
};
