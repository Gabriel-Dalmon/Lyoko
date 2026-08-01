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
#include "Engine/DataAsset.h"
#include "Gameplay/Items/ItemProperty.h"
#include "ItemDefinition.generated.h"

//----------------------------------------------------------------------------------------------------------------------
/**
 * @brief Represents the definition of an item, including its display name, description, icon, and associated properties.
 */
UCLASS(BlueprintType)
class LYOKOBASE_API UItemDefinition : public UPrimaryDataAsset
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

    //------------------------------------------------------------------------------------------------------------------
    // Item Properties
    //------------------------------------------------------------------------------------------------------------------
public:
    /**
     * @brief The properties associated with the item, mapped by their class types.
     */
    UPROPERTY(EditAnywhere, Instanced)
    TMap<TSubclassOf<UItemProperty>, TObjectPtr<UItemProperty>> Properties;

public:
    /**
     * @brief Checks if the item has a property of the specified class.
     * @param PropertyClass The class of the property to check for.
     * @return True if the item has the property, false otherwise.
     */
    UFUNCTION(BlueprintCallable, Category = "Gameplay|Item")
    bool HasProperty(TSubclassOf<UItemProperty> PropertyClass) const;

    /**
     * @brief Retrieves the property of the specified class from the item.
     * @param PropertyClass The class of the property to retrieve.
     * @return A pointer to the property if it exists, nullptr otherwise.
     */
    UFUNCTION(BlueprintCallable, Category = "Gameplay|Item")
    UItemProperty *GetProperty(TSubclassOf<UItemProperty> PropertyClass) const;

    /**
     * @brief Retrieves the property of the specified type from the item.
     * @tparam PropertyType The type of the property to retrieve.
     * @return A pointer to the property if it exists, nullptr otherwise.
     */
    template<typename PropertyType>
    PropertyType *GetProperty() const
    {
        static_assert(TIsDerivedFrom<PropertyType, UItemProperty>::IsDerived, "PropertyType must be derived from UItemProperty");
        return static_cast<PropertyType *>(Properties.Find(PropertyType::StaticClass()));
    }

    //------------------------------------------------------------------------------------------------------------------
    // Editor Properties Validation
    //------------------------------------------------------------------------------------------------------------------
#if WITH_EDITOR
private:
    /**
     * @brief Stores the previous state of the Properties map before an edit operation.
     * This is used to detect changes in the Properties map during editor operations.
     */
    TMap<TSubclassOf<UItemProperty>, TObjectPtr<UItemProperty>> PreviousProperties;

public:
    virtual void PreEditChange(FProperty *PropertyAboutToChange) override;
    virtual void PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent) override;
    virtual EDataValidationResult IsDataValid(class FDataValidationContext &Context) const override;

private:
    /**
     * @brief Called when a property key is changed in the Properties map.
     * @param Key The property key that was changed.
     * @param Value The property value associated with the changed key.
     */
    void OnPropertyKeyChanged(TSubclassOf<UItemProperty> &Key, TObjectPtr<UItemProperty> &Value);

    /**
     * @brief Called when a property value is changed in the Properties map.
     * @param Key The property key whose value was changed.
     * @param Value The new property value.
     */
    void OnPropertyValueChanged(TSubclassOf<UItemProperty> &Key, TObjectPtr<UItemProperty> &Value);
    
    /**
     * @brief Called when a property key is removed from the Properties map.
     * @param Key The property key that was removed.
     * @param Value The property value associated with the removed key.
     */
    void OnPropertyKeyEmptied(TSubclassOf<UItemProperty> &Key, TObjectPtr<UItemProperty> &Value);
    
    /**
     * @brief Called when a property value is removed from the Properties map.
     * @param Key The property key whose value was removed.
     * @param Value The removed property value.
     */
    void OnPropertyValueEmptied(TSubclassOf<UItemProperty> &Key, TObjectPtr<UItemProperty> &Value);

    /**
     * @brief Called when a property is added to the Properties map.
     * @param Key The property key that was added.
     * @param Value The property value associated with the added key.
     */
    void OnPropertyAdded(TSubclassOf<UItemProperty> &Key, TObjectPtr<UItemProperty> &Value);

    /**
     * @brief Called when a property is removed from the Properties map.
     * @param Key The property key that was removed.
     * @param Value The property value associated with the removed key.
     */
    void OnPropertyRemoved(TSubclassOf<UItemProperty> &Key, TObjectPtr<UItemProperty> &Value);

    /**
     * @brief Pushes an error notification to the editor with the specified message.
     * @param Message The error message to display.
     */
    void PushErrorNotification(FString Message);
#endif
};
