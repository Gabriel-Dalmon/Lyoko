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
#include "MultipleSchemasConsumer.generated.h"

//----------------------------------------------------------------------------------------------------------------------
/**
 * @brief Interface for actors that consume data from multiple centralized schemas.
 *
 * This interface extends the schema consumer pattern to support actors whose behavior
 * and data are driven by multiple ULyokoSchema data assets. It provides:
 *
 *   1. GetSchemaList() — returns the list of schema types this consumer uses
 *   2. GetSchemaInstance() — retrieves a schema instance by type
 *   3. HasSchema() — checks if a specific schema type is present
 *   4. IsSchemaValid() — validates all schemas meet requirements
 *   5. Property access helpers — default implementations forwarding to the appropriate schema
 *
 * Implementation pattern:
 *
 *   class AMyActor : public AActor, public IMultipleSchemasConsumer
 *   {
 *       GENERATED_BODY()
 *
 *   private:
 *       UPROPERTY(EditAnywhere, BlueprintReadOnly)
 *       TObjectPtr<USchemaA> SchemaA;
 *
 *       UPROPERTY(EditAnywhere, BlueprintReadOnly)
 *       TObjectPtr<USchemaB> SchemaB;
 *
 *       // Storage for schema instances keyed by type
 *       TMap<TSubclassOf<ULyokoSchema>, TObjectPtr<ULyokoSchema>> SchemaInstances;
 *
 *   public:
 *       // Register schemas in BeginPlay or constructor
 *       void RegisterSchema(ULyokoSchema* Schema)
 *       {
 *           if (Schema) SchemaInstances.Add(Schema->GetClass(), Schema);
 *       }
 *
 *       // Override the methods you need to customize
 *       virtual TArray<TSubclassOf<ULyokoSchema>> GetSchemaList() override
 *       {
 *           return { USchemaA::StaticClass(), USchemaB::StaticClass() };
 *       }
 *
 *       virtual ULyokoSchema* GetSchemaInstance(TSubclassOf<ULyokoSchema> SchemaClass) override
 *       {
 *           return SchemaInstances.FindRef(SchemaClass);
 *       }
 *
 *       virtual bool HasSchema(TSubclassOf<ULyokoSchema> SchemaClass) override
 *       {
 *           return SchemaInstances.Contains(SchemaClass);
 *       }
 *
 *       virtual bool IsSchemaValid() override
 *       {
 *           for (const auto& SchemaClass : GetSchemaList())
 *           {
 *               const auto Instance = GetSchemaInstance(SchemaClass);
 *               if (!Instance || !Instance->HasProperties(GetMandatoryPropertiesForSchema(SchemaClass)))
 *               {
 *                   return false;
 *               }
 *           }
 *           return true;
 *       }
 *
 *       // Optional: per-schema mandatory properties
 *       virtual TSet<TSubclassOf<ULyokoProperty>> GetMandatoryPropertiesForSchema(TSubclassOf<ULyokoSchema> SchemaClass)
 *       {
 *           if (SchemaClass == USchemaA::StaticClass())
 *           {
 *               return { UPropertyA::StaticClass() };
 *           }
 *           if (SchemaClass == USchemaB::StaticClass())
 *           {
 *               return { UPropertyB::StaticClass() };
 *           }
 *           return {};
 *       }
 *
 *       // Default impl helpers are available automatically
 *       // auto* prop = GetProperty<UPropertyA>(USchemaA::StaticClass());
 *       // bool has = HasProperty(UPropertyA::StaticClass());
 *   };
 */
UINTERFACE(MinimalAPI, BlueprintType)
class UMultipleSchemasConsumer : public UInterface
{
    GENERATED_BODY()
};

class LYOKOBASE_API IMultipleSchemasConsumer
{
    GENERATED_BODY()

public:
    //------------------------------------------------------------------------------------------------------------------
    // Core Methods — Override these in implementing classes
    //------------------------------------------------------------------------------------------------------------------

    /**
     * @brief Returns the list of schema types this consumer uses.
     * @return Array of schema class types required by this consumer.
     */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Schema")
    TArray<ULyokoSchema*> GetSchemaList() const;
    virtual TArray<ULyokoSchema *> GetSchemaList_Implementation() const = 0;

    /**
     * @brief Retrieves a schema instance by its class type.
     * @param SchemaClass The class type of the schema to retrieve.
     * @return The schema instance, or nullptr if not found.
     */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Schema")
    ULyokoSchema* GetSchema(TSubclassOf<ULyokoSchema> SchemaClass) const;
    virtual ULyokoSchema* GetSchema_Implementation(TSubclassOf<ULyokoSchema> SchemaClass) const;

    /**
     * @brief Checks if a specific schema type is present in this consumer.
     * @param SchemaClass The schema class type to check for.
     * @return True if the schema type is present.
     */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Schema")
    bool HasSchema(TSubclassOf<ULyokoSchema> SchemaClass) const;
    virtual bool HasSchema_Implementation(TSubclassOf<ULyokoSchema> SchemaClass) const;

    /**
     * @brief Returns the mandatory properties required by a specific schema type.
     * @param SchemaClass The schema class type to query.
     * @return Set of mandatory property classes.
     */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Schema")
    TSet<TSubclassOf<ULyokoProperty>> GetMandatoryPropertiesForSchema(TSubclassOf<ULyokoSchema> SchemaClass) const;
    virtual TSet<TSubclassOf<ULyokoProperty>> GetMandatoryPropertiesForSchema_Implementation(TSubclassOf<ULyokoSchema> SchemaClass) const = 0;

    /**
     * @brief Returns the optional properties used by a specific schema type.
     * These properties may or may not be present in the schema.
     * @param SchemaClass The schema class type to query.
     * @return Set of optional property classes.
     */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Schema")
    TSet<TSubclassOf<ULyokoProperty>> GetOptionalPropertiesForSchema(TSubclassOf<ULyokoSchema> SchemaClass) const;
    virtual TSet<TSubclassOf<ULyokoProperty>> GetOptionalPropertiesForSchema_Implementation(TSubclassOf<ULyokoSchema> SchemaClass) const;

    /**
     * @brief Validates that all schemas are valid and contain their required properties.
     * @return True if all schemas meet their requirements.
     */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Schema")
    bool IsSchemaValid(TSubclassOf<ULyokoSchema> SchemaClass) const;
    virtual bool IsSchemaValid_Implementation(TSubclassOf<ULyokoSchema> SchemaClass) const;

    //------------------------------------------------------------------------------------------------------------------
    // Default Implementations — Available to all consumers
    //------------------------------------------------------------------------------------------------------------------

    /**
     * @brief Gets a property of the specified type from a specific schema.
     * @tparam PropertyType The type of property to retrieve (must extend ULyokoProperty).
     * @param SchemaClass The schema class type to query.
     * @return Pointer to the property, or nullptr if not found.
     */
    template<typename PropertyType>
    PropertyType* GetProperty(TSubclassOf<ULyokoSchema> SchemaClass) const
    {
        static_assert(TIsDerivedFrom<PropertyType, ULyokoProperty>::IsDerived,
                      "PropertyType must extend ULyokoProperty");
        const auto Schema = GetSchema(SchemaClass);
        return Schema ? Schema->GetProperty<PropertyType>() : nullptr;
    }

    /**
     * @brief Gets a property by its class from the first matching schema.
     * @param PropertyClass The class of the property to retrieve.
     * @return Pointer to the property, or nullptr if not found in any schema.
     */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Schema")
    ULyokoProperty* GetProperty(TSubclassOf<ULyokoSchema> SchemaClass, TSubclassOf<ULyokoProperty> PropertyClass) const;
    virtual ULyokoProperty* GetProperty_Implementation(TSubclassOf<ULyokoSchema> SchemaClass, TSubclassOf<ULyokoProperty> PropertyClass) const;

    /**
     * @brief Checks if any schema contains the given property class.
     * @param PropertyClass The property class to check for.
     * @return True if any schema has the property.
     */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Schema")
    bool HasProperty(TSubclassOf<ULyokoSchema> SchemaClass, TSubclassOf<ULyokoProperty> PropertyClass) const;
    virtual bool HasProperty_Implementation(TSubclassOf<ULyokoSchema> SchemaClass, TSubclassOf<ULyokoProperty> PropertyClass) const;

    /**
     * @brief Returns all schema classes used by this consumer.
     * @return Array of schema class types.
     */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Schema")
    TArray<TSubclassOf<ULyokoSchema>> GetSchemaClasses() const;
    virtual TArray<TSubclassOf<ULyokoSchema>> GetSchemaClasses_Implementation() const;
};
