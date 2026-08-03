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
#include "DurabilityProperty.generated.h"

//----------------------------------------------------------------------------------------------------------------------
/**
 * @brief Represents the durability property of an item.
 */
UCLASS(BlueprintType, EditInlineNew)
class LYOKOBASE_API UDurabilityProperty : public ULyokoProperty
{
    GENERATED_BODY()
    
public:
    /**
     * @brief The current durability value of the item.
     */
    UPROPERTY(EditAnywhere, Category = "Durability Property")
    int MaximumDurability = 100;

    /**
     * @brief Checks if the durability property data is valid.
     * @param Context The data validation context.
     * @return The result of the validation.
     */
    virtual EDataValidationResult IsDataValid(class FDataValidationContext &Context) const override;
};
