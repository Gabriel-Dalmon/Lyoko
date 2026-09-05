// Copyright © 2025-2027 Lyoko - 96 l'Art Cheperdu

#include "Model/SchemaConsumer.h"

//----------------------------------------------------------------------------------------------------------------------
TSet<TSubclassOf<ULyokoProperty>> ISchemaConsumer::GetOptionalProperties_Implementation() const
{
    return {};
}

//----------------------------------------------------------------------------------------------------------------------
ULyokoProperty* ISchemaConsumer::GetProperty_Implementation(TSubclassOf<ULyokoProperty> PropertyClass) const
{
    const auto Schema = GetSchema();
    return Schema ? Schema->GetProperty(PropertyClass) : nullptr;
}

//----------------------------------------------------------------------------------------------------------------------
bool ISchemaConsumer::HasProperty_Implementation(TSubclassOf<ULyokoProperty> PropertyClass) const
{
    const auto Schema = GetSchema();
    return Schema ? Schema->HasProperty(PropertyClass) : false;
}

//----------------------------------------------------------------------------------------------------------------------
bool ISchemaConsumer::HasAllProperties_Implementation(const TSet<TSubclassOf<ULyokoProperty>>& PropertyClasses) const
{
    const auto Schema = GetSchema();
    return Schema ? Schema->HasProperties(PropertyClasses) : false;
}

//----------------------------------------------------------------------------------------------------------------------
bool ISchemaConsumer::HasAnyProperty_Implementation(const TSet<TSubclassOf<ULyokoProperty>>& PropertyClasses) const
{
    const auto Schema = GetSchema();
    return Schema ? Schema->HasAnyProperty(PropertyClasses) : false;
}

//----------------------------------------------------------------------------------------------------------------------
TSubclassOf<ULyokoSchema> ISchemaConsumer::GetSchemaClass_Implementation() const
{
    const auto Schema = GetSchema();
    return Schema ? Schema->GetClass() : nullptr;
}
