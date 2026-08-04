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
#include "Model/LyokoProperty.h"
#include "LyokoSchema.generated.h"

//----------------------------------------------------------------------------------------------------------------------
/**
 * @brief Base setup to leverage composition when creating data asset classes.
 */
UCLASS(BlueprintType)
class LYOKOBASE_API ULyokoSchema : public UPrimaryDataAsset
{
    GENERATED_BODY()

    //------------------------------------------------------------------------------------------------------------------
    // Properties Access
    //------------------------------------------------------------------------------------------------------------------
public:
    /**
     * @brief The properties container of the schema, allowing for dynamic composition of properties.
     */
    UPROPERTY(EditAnywhere, Instanced)
    TMap<TSubclassOf<ULyokoProperty>, TObjectPtr<ULyokoProperty>> Properties;

public:
    /**
     * @brief Checks if the schema has a property of the specified class.
     * @param PropertyClass The class of the property to check for.
     * @return True if the schema has the property, false otherwise.
     */
    UFUNCTION(BlueprintCallable, Category = "Data|Property")
    bool HasProperty(TSubclassOf<ULyokoProperty> PropertyClass) const;

    /**
     * @brief Checks if the schema has all the specified properties.
     * @param PropertyClasses The set of property classes to check for.
     * @return True if the schema has all the properties, false otherwise.
     */
    UFUNCTION(BlueprintCallable, Category = "Data|Property")
    bool HasProperties(const TSet<TSubclassOf<ULyokoProperty>> &PropertyClasses) const;

    /**
     * @brief Checks if the schema has any of the specified properties.
     * @param PropertyClasses The set of property classes to check for.
     * @return True if the schema has any of the properties, false otherwise.
     */
    UFUNCTION(BlueprintCallable, Category = "Data|Property")
    bool HasAnyProperty(const TSet<TSubclassOf<ULyokoProperty>> &PropertyClasses) const;

    /**
     * @brief Retrieves the property of the specified class from the schema.
     * @param PropertyClass The class of the property to retrieve.
     * @return A pointer to the property if it exists, nullptr otherwise.
     */
    UFUNCTION(BlueprintCallable, Category = "Data|Property")
    ULyokoProperty *GetProperty(TSubclassOf<ULyokoProperty> PropertyClass) const;

    /**
     * @brief Retrieves the property of the specified type from the schema.
     * @tparam PropertyType The type of the property to retrieve.
     * @return A pointer to the property if it exists, nullptr otherwise.
     */
    template<typename PropertyType>
    PropertyType *GetProperty() const
    {
        static_assert(TIsDerivedFrom<PropertyType, ULyokoProperty>::IsDerived, "PropertyType must be derived from ULyokoProperty");
        return static_cast<PropertyType *>(Properties.FindRef(PropertyType::StaticClass()));
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
    TMap<TSubclassOf<ULyokoProperty>, TObjectPtr<ULyokoProperty>> PreviousProperties;

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
    void OnPropertyKeyChanged(TSubclassOf<ULyokoProperty> &Key, TObjectPtr<ULyokoProperty> &Value);

    /**
     * @brief Called when a property value is changed in the Properties map.
     * @param Key The property key whose value was changed.
     * @param Value The new property value.
     */
    void OnPropertyValueChanged(TSubclassOf<ULyokoProperty> &Key, TObjectPtr<ULyokoProperty> &Value);

    /**
     * @brief Called when a property key is removed from the Properties map.
     * @param Key The property key that was removed.
     * @param Value The property value associated with the removed key.
     */
    void OnPropertyKeyEmptied(TSubclassOf<ULyokoProperty> &Key, TObjectPtr<ULyokoProperty> &Value);

    /**
     * @brief Called when a property value is removed from the Properties map.
     * @param Key The property key whose value was removed.
     * @param Value The removed property value.
     */
    void OnPropertyValueEmptied(TSubclassOf<ULyokoProperty> &Key, TObjectPtr<ULyokoProperty> &Value);

    /**
     * @brief Called when a property is added to the Properties map.
     * @param Key The property key that was added.
     * @param Value The property value associated with the added key.
     */
    void OnPropertyAdded(TSubclassOf<ULyokoProperty> &Key, TObjectPtr<ULyokoProperty> &Value);

    /**
     * @brief Called when a property is removed from the Properties map.
     * @param Key The property key that was removed.
     * @param Value The property value associated with the removed key.
     */
    void OnPropertyRemoved(TSubclassOf<ULyokoProperty> &Key, TObjectPtr<ULyokoProperty> &Value);

    /**
     * @brief Pushes an error notification to the editor with the specified message.
     * @param Message The error message to display.
     */
    void PushErrorNotification(FString Message);
#endif
};
