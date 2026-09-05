// Copyright © 2025-2027 Lyoko - 96 l'Art Cheperdu
//     __                __       
//    / /   __  ______  / /______ 
//   / /   / / / / __ \/ //_/ / 
//  / /___/ /_/ / /_/ / ,< / /_/ /
// /_____/\__, /\____/_/|_|\____/ 
//       /____/                   

#pragma once

//----------------------------------------------------------------------------------------------------------------------
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Model/LyokoSchema.h"
#include "Model/LyokoProperty.h"
#include "SchemaConsumer.generated.h"

//----------------------------------------------------------------------------------------------------------------------
/**
 * @brief Interface for actors that consume data from a single centralized schema.
 *
 * This interface defines the contract for actors whose behavior and data are driven
 * by a single ULyokoSchema data asset. It provides:
 *
 *   1. GetSchema() — returns the schema reference (default: nullptr)
 *   2. GetMandatoryProperties() — returns required properties (default: empty set)
 *   3. GetOptionalProperties() — returns optional properties (default: empty set)
 *   4. IsSchemaValid() — validates schema meets requirements (default: false)
 *   5. Property access helpers — default implementations forwarding to the schema
 *
 * Implementation pattern:
 *
 *   class AMyActor : public AActor, public ISchemaConsumer
 *   {
 *       GENERATED_BODY()
 *
 *   private:
 *       UPROPERTY(EditAnywhere, BlueprintReadOnly)
 *       TObjectPtr<UMySchema> MySchema;
 *
 *   public:
 *       // Override the methods you need to customize
 *       virtual ULyokoSchema* GetSchema() override { return MySchema; }
 *       virtual TSet<TSubclassOf<ULyokoProperty>> GetMandatoryProperties() override
 *       {
 *           return { UMyProperty::StaticClass() };
 *       }
 *       virtual bool IsSchemaValid() override
 *       {
 *           if (!MySchema) return false;
 *           return MySchema->HasProperties(GetMandatoryProperties());
 *       }
 *
 *       // Default impl helpers are available automatically
 *       // auto* prop = GetProperty<UMyProperty>();
 *       // bool has = HasProperty(UMyProperty::StaticClass());
 *   };
 */
UINTERFACE(MinimalAPI, BlueprintType)
class USchemaConsumer : public UInterface
{
    GENERATED_BODY()
};

class LYOKOBASE_API ISchemaConsumer
{
    GENERATED_BODY()

public:
    //------------------------------------------------------------------------------------------------------------------
    // Core Methods — Override these in implementing classes
    //------------------------------------------------------------------------------------------------------------------

    /**
     * @brief Returns the schema reference used by this consumer.
     * @return The schema data asset, or nullptr if not set.
     */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Schema")
    ULyokoSchema *GetSchema() const;
    virtual ULyokoSchema *GetSchema_Implementation() const = 0;

    /**
     * @brief Returns the mandatory properties required by this consumer.
     * These properties must be present in the schema for IsSchemaValid() to return true.
     * @return Set of required property classes.
     */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Schema")
    TSet<TSubclassOf<ULyokoProperty>> GetMandatoryProperties() const;
    virtual TSet<TSubclassOf<ULyokoProperty>> GetMandatoryProperties_Implementation() const = 0;

    /**
     * @brief Returns the optional properties used by this consumer.
     * These properties may or may not be present in the schema.
     * @return Set of optional property classes.
     */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Schema")
    TSet<TSubclassOf<ULyokoProperty>> GetOptionalProperties() const;
    virtual TSet<TSubclassOf<ULyokoProperty>> GetOptionalProperties_Implementation() const;

    /**
     * @brief Validates that the schema is valid (non-null) and contains all mandatory properties.
     * @return True if the schema meets all requirements.
     */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Schema")
    bool IsSchemaValid();
    virtual bool IsSchemaValid_Implementation() //TODO@g: move to .cpp
    {
        const auto Schema = GetSchema();
        if (!Schema)
        {
            return false;
        }
        return Schema->HasProperties(GetMandatoryProperties());
    }

    //------------------------------------------------------------------------------------------------------------------
    // Default Implementations — Available to all consumers
    //------------------------------------------------------------------------------------------------------------------

    /**
     * @brief Gets a property of the specified type from the schema.
     * @tparam PropertyType The type of property to retrieve (must extend ULyokoProperty).
     * @return Pointer to the property, or nullptr if not found.
     */
    template<typename PropertyType>
    PropertyType* GetProperty() const
    {
        static_assert(TIsDerivedFrom<PropertyType, ULyokoProperty>::IsDerived,
                      "PropertyType must extend ULyokoProperty");
        const auto Schema = GetSchema();
        return Schema ? Schema->GetProperty<PropertyType>() : nullptr;
    }

    /**
     * @brief Gets a property by its class from the schema.
     * @param PropertyClass The class of the property to retrieve.
     * @return Pointer to the property, or nullptr if not found.
     */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Schema")
    ULyokoProperty* GetProperty(TSubclassOf<ULyokoProperty> PropertyClass) const;
    virtual ULyokoProperty* GetProperty_Implementation(TSubclassOf<ULyokoProperty> PropertyClass) const;

    /**
     * @brief Checks if the schema contains the given property class.
     * @param PropertyClass The property class to check for.
     * @return True if the property exists in the schema.
     */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Schema")
    bool HasProperty(TSubclassOf<ULyokoProperty> PropertyClass) const;
    virtual bool HasProperty_Implementation(TSubclassOf<ULyokoProperty> PropertyClass) const;

    /**
     * @brief Checks if the schema contains all the given property classes.
     * @param PropertyClasses The set of property classes to check for.
     * @return True if all properties exist in the schema.
     */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Schema")
    bool HasAllProperties(const TSet<TSubclassOf<ULyokoProperty>>& PropertyClasses) const;
    virtual bool HasAllProperties_Implementation(const TSet<TSubclassOf<ULyokoProperty>>& PropertyClasses) const;

    /**
     * @brief Checks if the schema contains any of the given property classes.
     * @param PropertyClasses The set of property classes to check for.
     * @return True if any property exists in the schema.
     */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Schema")
    bool HasAnyProperty(const TSet<TSubclassOf<ULyokoProperty>>& PropertyClasses) const;
    virtual bool HasAnyProperty_Implementation(const TSet<TSubclassOf<ULyokoProperty>>& PropertyClasses) const;

    /**
     * @brief Returns the schema's UClass* (useful for logging, debugging, type checks).
     * @return The schema class, or nullptr if schema is not set.
     */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Schema")
    TSubclassOf<ULyokoSchema> GetSchemaClass() const;
    virtual TSubclassOf<ULyokoSchema> GetSchemaClass_Implementation() const;
};
