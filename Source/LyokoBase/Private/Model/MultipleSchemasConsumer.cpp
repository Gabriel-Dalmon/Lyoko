// Copyright © 2025-2027 Lyoko - 96 l'Art Cheperdu

#include "Model/MultipleSchemasConsumer.h"

//----------------------------------------------------------------------------------------------------------------------
ULyokoSchema* IMultipleSchemasConsumer::GetSchema_Implementation(TSubclassOf<ULyokoSchema> SchemaClass) const
{
    const auto& SchemaList = GetSchemaList();
    const auto Schema = *SchemaList.FindByPredicate([SchemaClass](ULyokoSchema *Schema)
        {
            return Schema && Schema->GetClass() == SchemaClass;
        });
    return Schema;
}

//----------------------------------------------------------------------------------------------------------------------
bool IMultipleSchemasConsumer::HasSchema_Implementation(TSubclassOf<ULyokoSchema> SchemaClass) const
{
    const auto &SchemaList = GetSchemaList();
    return SchemaList.ContainsByPredicate([SchemaClass](ULyokoSchema *Schema)
        {
            return Schema && Schema->GetClass() == SchemaClass;
        });
}

//----------------------------------------------------------------------------------------------------------------------
TSet<TSubclassOf<ULyokoProperty>> IMultipleSchemasConsumer::GetOptionalPropertiesForSchema_Implementation(TSubclassOf<ULyokoSchema> SchemaClass) const
{
    return {};
}

//----------------------------------------------------------------------------------------------------------------------
bool IMultipleSchemasConsumer::IsSchemaValid_Implementation(TSubclassOf<ULyokoSchema> SchemaClass) const
{
    const auto Instance = GetSchema(SchemaClass);
    return Instance && Instance->HasProperties(GetMandatoryPropertiesForSchema(SchemaClass));
}

//----------------------------------------------------------------------------------------------------------------------
ULyokoProperty* IMultipleSchemasConsumer::GetProperty_Implementation(TSubclassOf<ULyokoSchema> SchemaClass, TSubclassOf<ULyokoProperty> PropertyClass) const
{
    const auto Schema = GetSchema(SchemaClass);
    return Schema ? Schema->GetProperty(PropertyClass) : nullptr;
}

//----------------------------------------------------------------------------------------------------------------------
bool IMultipleSchemasConsumer::HasProperty_Implementation(TSubclassOf<ULyokoSchema> SchemaClass, TSubclassOf<ULyokoProperty> PropertyClass) const
{
    const auto Schema = GetSchema(SchemaClass);
    return Schema ? Schema->HasProperty(PropertyClass) : false;
}

//----------------------------------------------------------------------------------------------------------------------
TArray<TSubclassOf<ULyokoSchema>> IMultipleSchemasConsumer::GetSchemaClasses_Implementation() const
{
    const auto &SchemaList = GetSchemaList();
    TArray<TSubclassOf<ULyokoSchema>> Result;
    for (const auto &Schema : SchemaList)
    {
        Result.Add(Schema->GetClass());
    }
    return Result;
}
