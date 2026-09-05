# Gameplay Data Architecture

## Overview

The Lyoko game uses a **composition-based DataAsset architecture** built on `ULyokoSchema` and `ULyokoProperty`. This system provides a modular, single-source-of-truth approach to game data management where each entity (items, weapons, projectiles, etc.) is defined by a data asset that composes only the properties it needs.

Actors consume schema data through standardized interfaces (`ISchemaConsumer`, `IMultipleSchemaConsumer`) that define the contract between the actor and its data source.

This document describes the core architecture, actor interfaces, design rationale, usage patterns, and all concrete implementations within the project.

---

## Core Classes

### ULyokoSchema (Base Schema)

```
ULyokoSchema (UCLASS(BlueprintType))
  └─ extends UPrimaryDataAsset
```

The foundational schema class that all game data assets inherit from. It provides the composition infrastructure:

| Member | Type | Purpose |
|--------|------|---------|
| `Properties` | `TMap<TSubclassOf<ULyokoProperty>, TObjectPtr<ULyokoProperty>>` | The composition container. Key = property class, Value = property instance. |

**Key Methods:**

| Method | Return | Description |
|--------|--------|-------------|
| `HasProperty(TSubclassOf<ULyokoProperty>)` | `bool` | Checks if a property of the given class exists in the map. |
| `HasProperties(TSet<TSubclassOf<ULyokoProperty>>)` | `bool` | Checks if **all** given property classes exist. |
| `HasAnyProperty(TSet<TSubclassOf<ULyokoProperty>>)` | `bool` | Checks if **any** of the given property classes exist. |
| `GetProperty(TSubclassOf<ULyokoProperty>)` | `ULyokoProperty*` | Retrieves a property by class. Returns nullptr if not found. |
| `GetProperty<T>()` | `T*` | Template-based typed access. Returns nullptr if not found. |

**Editor Features:**
- `PreEditChange` / `PostEditChangeProperty` — detects map mutations and dispatches lifecycle events
- `IsDataValid` — validates that keys match values, no null entries, and each property passes its own validation
- `OnPropertyAdded` / `OnPropertyRemoved` — hook points for custom editor logic
- `OnPropertyKeyChanged` — auto-recreates the property object when the key class changes
- `OnPropertyValueChanged` — prevents duplicate property types (one instance per type)
- `PushErrorNotification` — displays transient Slate notifications on validation errors

### ULyokoProperty (Base Property)

```
ULyokoProperty (UCLASS(Abstract, BlueprintType, Blueprintable, EditInlineNew, DefaultToInstanced))
  └─ extends UObject
```

The base class for all schema properties. Key specifiers:

| Specifier | Effect |
|-----------|--------|
| `Abstract` | Cannot be instantiated directly in the editor — must be subclassed |
| `BlueprintType` | Exposed to Blueprint as a type |
| `Blueprintable` | Can be subclassed in Blueprint |
| `EditInlineNew` | Can be created inline within the parent schema's editor |
| `DefaultToInstanced` | Automatically instanced when added to a schema |

**Key Method:**
- `IsDataValid(FDataValidationContext&)` — override to validate property data in the editor

---

## Actor Interfaces

Actors consume schema data through one of two interfaces. Both provide pure virtual methods that define the contract between the actor and its data source, plus default implementations for common operations.

**Naming convention:**
- File: no `I` prefix → `SchemaConsumer.h`, `MultipleSchemaConsumer.h`
- UCLASS: no `I` prefix → `USchemaConsumer`, `UMultipleSchemaConsumer`
- Interface class: single `I` prefix → `ISchemaConsumer`, `IMultipleSchemaConsumer`

### ISchemaConsumer (Single Schema)

`ISchemaConsumer` (header: `SchemaConsumer.h`, UCLASS: `USchemaConsumer`)

Interface for actors that consume data from a **single** centralized schema.

**Pure Virtual Methods (must be implemented):**

| Method | Return | Purpose |
|--------|--------|---------|  
| `GetSchema()` | `ULyokoSchema*` | Returns the schema reference |
| `GetMandatoryProperties()` | `TSet<TSubclassOf<ULyokoProperty>>` | Declares required properties |
| `GetOptionalProperties()` | `TSet<TSubclassOf<ULyokoProperty>>` | Declares optional properties |
| `IsSchemaValid()` | `bool` | Validates schema meets requirements |

**Default Implementations (available to all consumers):**

| Method | Return | Purpose |
|--------|--------|---------|  
| `GetProperty<T>()` | `T*` | Typed property access |
| `GetProperty(PropertyClass)` | `ULyokoProperty*` | Generic property access |
| `HasProperty(PropertyClass)` | `bool` | Checks if property exists |
| `HasAllProperties(Classes)` | `bool` | Checks if all properties exist |
| `HasAnyProperty(Classes)` | `bool` | Checks if any property exists |
| `GetSchemaClass()` | `TSubclassOf<ULyokoSchema>` | Returns schema's UClass* |

### IMultipleSchemaConsumer (Multiple Schemas)

`IMultipleSchemaConsumer` (header: `MultipleSchemaConsumer.h`, UCLASS: `UMultipleSchemaConsumer`)

Interface for actors that consume data from **multiple** centralized schemas.

**Pure Virtual Methods (must be implemented):**

| Method | Return | Purpose |
|--------|--------|---------|  
| `GetSchemaList()` | `TArray<TSubclassOf<ULyokoSchema>>` | Returns list of schema types |
| `GetSchemaInstance(SchemaClass)` | `ULyokoSchema*` | Retrieves schema instance by type |
| `HasSchema(SchemaClass)` | `bool` | Checks if schema type is present |
| `IsSchemaValid()` | `bool` | Validates all schemas meet requirements |

**Default Implementations (available to all consumers):**

| Method | Return | Purpose |
|--------|--------|---------|  
| `GetProperty<T>(SchemaClass)` | `T*` | Typed property access for specific schema |
| `GetProperty(PropertyClass)` | `ULyokoProperty*` | Generic property access (first matching schema) |
| `HasProperty(PropertyClass)` | `bool` | Checks if any schema has the property |
| `GetSchemaClass()` | `TArray<TSubclassOf<ULyokoSchema>>` | Alias for `GetSchemaList()` |

### When to Use Which Interface

| Scenario | Interface |
|----------|-----------|
| Actor has one data source (e.g., `AItem` with `UItemSchema`) | `ISchemaConsumer` |
| Actor combines data from multiple schemas (e.g., weapon stats + projectile config) | `IMultipleSchemaConsumer` |
| Actor needs to query properties from different schemas independently | `IMultipleSchemaConsumer` |
| Actor's data is entirely contained in one schema | `ISchemaConsumer` |

---

## Architecture Diagram

```
┌─────────────────────────────────────────────────────────────────────────────┐
│                        LYOKO DATA ARCHITECTURE                              │
│                                                                             │
│  ULyokoSchema (UPrimaryDataAsset)                                           │
│  ┌─────────────────────────────────────────────────────────────────────┐    │
│  │  Properties: TMap<TSubclassOf<ULyokoProperty>, TObjectPtr<...>>    │    │
│  │  [Composition container — each schema composes its own set]        │    │
│  └─────────────────────────────────────────────────────────────────────┘    │
│       │                                                                      │
│       ├─ HasProperty() / HasProperties() / HasAnyProperty()                 │
│       ├─ GetProperty<T>() — typed access                                    │
│       └─ Editor validation hooks                                            │
│                                                                             │
└─────────────────────────────────────────────────────────────────────────────┘
       │                                                                      │
       ├──────────────────────────────────────────────────────────────────────┤
       │                                                                      │
       ▼                                                                      ▼
┌──────────────────────────────┐  ┌──────────────────────────────────────────────┐
│ UItemSchema                  │  │ UProjectileSchema                            │
│ (extends ULyokoSchema)       │  │ (extends ULyokoSchema)                       │
│ ┌────────────────────────┐   │  │ ┌────────────────────────────────────────┐   │
│ │ DisplayName: FText     │   │  │ │ DisplayName: FText                     │   │
│ │ Description: FText     │   │  │ │ Description: FText                     │   │
│ │ Icon: UTexture2D*      │   │  │ │ [Composition via Properties map]       │   │
│ │ [Composition via       │   │  │ └────────────────────────────────────────┘   │
│ │  Properties map]       │   │  │                                              │
│ └────────────────────────┘   │  │                                              │
│                              │  │  Example composition:                        │
│  Used by: AItem              │  │  Properties:                                 │
│                              │  │    UProjectileDamageProperty → { Damage=25 }  │
│                              │  │    UProjectileRadiusProperty → { Radius=8 }   │
│                              │  │    UProjectileOnHitBehaviorProperty → Destroy │
│                              │  │    UProjectileImpulseProperty → { Impulse=150 }│
└──────────────────────────────┘  └──────────────────────────────────────────────┘
       │                                       │
       │  Properties used by AItem:            │  Properties used by AProjectileBase:
       │  ┌──────────────────────────────┐     │  ┌──────────────────────────────────────┐
       │  │ UDurabilityProperty          │     │  │ UProjectileDamageProperty          │
       │  │   MaximumDurability: int     │     │  │   Damage: float                    │
       │  │  (required by AItem)         │     │  │ UProjectileRadiusProperty          │
       │  └──────────────────────────────┘     │  │   Radius: float                    │
       │                                      │  │ UProjectileOnHitBehaviorProperty   │
       │  Used by: AWeaponBase (extends AItem)│  │   OnHitBehavior: EProjectileOnHitBehavior
       │                                      │  │ UProjectileImpulseProperty         │
       │                                      │  │   ImpulseStrength: float           │
       │                                      │  │   MasterFieldClass: TSubclassOf<AActor>
       │                                      │  └──────────────────────────────────────┘
       └──────────────────────────────────────┘
```

---

## Design Rationale

### Composition Over Inheritance

**Before (inheritance-based):**
```
UProjectileDataBase
├── Damage
├── Radius
└── OnHitBehavior

UImpulseProjectileData (extends UProjectileDataBase)
├── ImpulseStrength
└── MasterFieldClass
```
Every new variant required a new subclass, leading to combinatorial explosion.

**After (composition-based):**
```
UProjectileSchema (ULyokoSchema)
└── Properties: [any combination of property types]
```
Each data asset composes only what it needs. New property types can be added without touching the schema.

### Single Source of Truth

All game data lives in DataAssets, not hardcoded in actors. Actors hold a reference to their schema and query properties at runtime:

```cpp
// Actor holds schema reference
UProjectileSchema* Schema = GetProjectileSchema();

// Typed property access
const auto DamageProp = Schema->GetProperty<UProjectileDamageProperty>();
const float Damage = DamageProp ? DamageProp->Damage : 0.0f;
```

### Actor Interface Contract

The `ISchemaConsumer` / `IMultipleSchemaConsumer` interfaces formalize the contract between actors and schemas:

```
Actor (ISchemaConsumer)
├── GetSchema() → returns schema reference
├── GetMandatoryProperties() → declares what it needs
├── GetOptionalProperties() → declares what it optionally uses
└── IsSchemaValid() → validates schema meets requirements
    └── Schema->HasProperties(GetMandatoryProperties())
```

The actor **declares** its requirements via `GetMandatoryProperties()`. The schema **provides** the data via its `Properties` map. `IsSchemaValid()` bridges the two by checking that the schema contains all mandatory properties.

---

## Property Classes

### UDurabilityProperty (LyokoBase)
- **Purpose**: Tracks item durability
- **Field**: `int MaximumDurability` (default: 100)
- **Used by**: `AItem` (via `UItemSchema`)
- **Validation**: Ensures `MaximumDurability > 0`

### UDamageProperty (LyokoBase)
- **Purpose**: Base damage property
- **Field**: *(empty — placeholder for future use)*
- **Used by**: Currently not composed by any schema

### UAttackCooldownProperty (LyokoWeapons)
- **Purpose**: Attack cooldown duration
- **Field**: `float AttackCooldown` (default: 1.0f)
- **Used by**: `AWeaponBase` (via `UItemSchema` composition)

### UDamageModifierProperty (LyokoWeapons)
- **Purpose**: Scales damage dealt by a weapon
- **Field**: `float DamageModifier` (default: 1.0f)
- **Used by**: `ARangedWeapon` (via `UItemSchema` composition)

### UReloadProperty (LyokoWeapons)
- **Purpose**: Weapon ammunition reload configuration
- **Fields**: Capacity, ReloadBatchSize, ReloadDuration, ReloadPattern
- **Used by**: `ARangedWeapon` (via `UItemSchema` composition)

### UProjectileProperty (LyokoWeapons)
- **Purpose**: References the projectile class to spawn
- **Field**: `TSubclassOf<AProjectileBase> ProjectileClass`
- **Used by**: `ARangedWeapon` (via `UItemSchema` composition)

### UProjectileDamageProperty (LyokoWeapons)
- **Purpose**: Base damage value for a projectile
- **Field**: `float Damage` (default: 10.0f)
- **Used by**: `AProjectileBase` (via `UProjectileSchema` composition)

### UProjectileRadiusProperty (LyokoWeapons)
- **Purpose**: Collision sphere radius for a projectile
- **Field**: `float Radius` (default: 5.0f)
- **Used by**: `AProjectileBase` (via `UProjectileSchema` composition)

### UProjectileOnHitBehaviorProperty (LyokoWeapons)
- **Purpose**: Behavior on projectile hit
- **Field**: `EProjectileOnHitBehavior OnHitBehavior` (default: Destroy)
- **Used by**: `AProjectileBase` (via `UProjectileSchema` composition)

### UProjectileImpulseProperty (LyokoWeapons)
- **Purpose**: Impulse force and optional Master Field spawning
- **Fields**:
  - `float ImpulseStrength` (default: 100.0f)
  - `TSubclassOf<AActor> MasterFieldClass` (default: nullptr)
- **Used by**: `AImpulseProjectile` (via `UProjectileSchema` composition)

---

## Runtime Data Flow

### Item Example

```
AItem (implements ISchemaConsumer)
  └── ItemDefinition: UItemSchema*
       └── Properties:
            ├── UDurabilityProperty → { MaximumDurability = 100 }

Runtime:
  AItem::GetProperty<UDurabilityProperty>()
    → ItemDefinition->GetProperty<UDurabilityProperty>()
    → Properties.FindRef(UDurabilityProperty::StaticClass())
    → returns UDurabilityProperty*
```

### Weapon Example

```
ARangedWeapon (extends AWeaponBase, extends AItem)
  └── ItemDefinition: UItemSchema*
       └── Properties:
            ├── UDurabilityProperty → { MaximumDurability = 200 }
            ├── UAttackCooldownProperty → { AttackCooldown = 0.5 }
            ├── UReloadProperty → { Capacity = 30, ... }
            ├── UProjectileProperty → { ProjectileClass = AImpulseProjectile }
            └── UDamageModifierProperty → { DamageModifier = 1.5 }

Runtime:
  ARangedWeapon::GetProperty<UProjectileProperty>()
    → ItemDefinition->GetProperty<UProjectileProperty>()
    → Properties.FindRef(UProjectileProperty::StaticClass())
    → returns UProjectileProperty*
    → .ProjectileClass (used to spawn actor)
```

### Projectile Example

```
AImpulseProjectile (extends AProjectileBase, implements ISchemaConsumer)
  └── ProjectileSchema: UProjectileSchema*
       └── Properties:
            ├── UProjectileDamageProperty → { Damage = 25.0 }
            ├── UProjectileRadiusProperty → { Radius = 8.0 }
            ├── UProjectileOnHitBehaviorProperty → { OnHitBehavior = Destroy }
            └── UProjectileImpulseProperty → { ImpulseStrength = 150.0 }

Runtime:
  AProjectileBase::GetDamage()
    → ProjectileSchema->GetProperty<UProjectileDamageProperty>()
    → .Damage (used in ApplyDamages())
```

### Interface Contract in Action

```
Actor (ISchemaConsumer)                    Schema (DataAsset)
─────────────────────────                   ─────────────────
GetSchema() → ItemDefinition               Properties map
GetMandatoryProperties() → {UDurability}   [UDurabilityProperty]
IsSchemaValid() → HasProperties({UDurability}) → true
GetProperty<UDurabilityProperty>() → .MaximumDurability
```

The actor declares what it needs → the schema provides it → `IsSchemaValid()` ensures the contract is fulfilled.

---

## Creating New Data Assets

1. **In UE5 Editor**: Content Browser → right-click → **Miscellaneous → Data Asset**
2. **Select class**: Choose the appropriate schema (`UItemSchema`, `UProjectileSchema`, etc.)
3. **Compose properties**: In the Details panel, expand **Properties** → click **Add Property**
4. **Pick property type**: Select from the list of available `ULyokoProperty` subclasses
5. **Set fields**: Configure each property's fields in the Details panel
6. **Fill metadata**: Set DisplayName, Description, Icon, etc.

## Creating New Property Types

```cpp
#pragma once
#include "CoreMinimal.h"
#include "Model/LyokoProperty.h"
#include "MyNewProperty.generated.h"

/**
 * @brief Description of what this property does.
 */
UCLASS()
class MYMODULE_API UMyNewProperty : public ULyokoProperty
{
    GENERATED_BODY()

public:
    /**
     * @brief Description of the field.
     */
    UPROPERTY(EditAnywhere, Category = "My Property")
    float MyValue = 1.0f;

    /**
     * @brief Override to validate this property's data.
     */
    virtual EDataValidationResult IsDataValid(class FDataValidationContext &Context) const override
    {
        return MyValue > 0.0f ? EDataValidationResult::Valid : EDataValidationResult::Invalid;
    }
};
```

## Creating New Schema Types

```cpp
#pragma once
#include "CoreMinimal.h"
#include "Model/LyokoSchema.h"
#include "MySchema.generated.h"

UCLASS(BlueprintType)
class MYMODULE_API UMySchema : public ULyokoSchema
{
    GENERATED_BODY()

public:
    /**
     * @brief Display name.
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Display")
    FText DisplayName;

    /**
     * @brief Description.
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Display")
    FText Description;
};
```

## Creating Actors with Schema References

Actors should implement `ISchemaConsumer` (single schema) or `IMultipleSchemaConsumer` (multiple schemas) to establish the contract with the data system.

### Single Schema Actor (ISchemaConsumer)

```cpp
UCLASS()
class AMyActor : public AActor, public ISchemaConsumer
{
    GENERATED_BODY()

private:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
    TObjectPtr<UMySchema> MySchema;

public:
    /** Returns the schema reference */
    inline UMySchema *GetMySchema() const { return MySchema; }

    // Pure virtual overrides
    virtual ULyokoSchema* GetSchema_Implementation() const override { return MySchema; }

    virtual TSet<TSubclassOf<ULyokoProperty>> GetMandatoryProperties_Implementation() const override
    {
        return { UMyProperty::StaticClass() };
    }

    virtual TSet<TSubclassOf<ULyokoProperty>> GetOptionalProperties_Implementation() const override
    {
        return {};
    }

    virtual bool IsSchemaValid_Implementation() const override
    {
        if (!MySchema) return false;
        return MySchema->HasProperties(GetMandatoryProperties());
    }

    // Default impl helpers are available automatically:
    // auto* prop = GetProperty<UMyProperty>();
    // bool has = HasProperty(UMyProperty::StaticClass());
    // bool hasAll = HasAllProperties({ UPropA::StaticClass(), UPropB::StaticClass() });
};
```

### Multiple Schema Actor (IMultipleSchemaConsumer)

```cpp
UCLASS()
class AMyMultiActor : public AActor, public IMultipleSchemaConsumer
{
    GENERATED_BODY()

private:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TObjectPtr<USchemaA> SchemaA;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TObjectPtr<USchemaB> SchemaB;

    // Storage for schema instances keyed by type
    TMap<TSubclassOf<ULyokoSchema>, TObjectPtr<ULyokoSchema>> SchemaInstances;

public:
    void RegisterSchema(ULyokoSchema* Schema)
    {
        if (Schema) SchemaInstances.Add(Schema->GetClass(), Schema);
    }

    // Pure virtual overrides
    virtual TArray<TSubclassOf<ULyokoSchema>> GetSchemaList_Implementation() const override
    {
        return { USchemaA::StaticClass(), USchemaB::StaticClass() };
    }

    virtual ULyokoSchema* GetSchemaInstance_Implementation(TSubclassOf<ULyokoSchema> SchemaClass) const override
    {
        return SchemaInstances.FindRef(SchemaClass);
    }

    virtual bool HasSchema_Implementation(TSubclassOf<ULyokoSchema> SchemaClass) const override
    {
        return SchemaInstances.Contains(SchemaClass);
    }

    virtual bool IsSchemaValid_Implementation() const override
    {
        for (const auto& SchemaClass : GetSchemaList())
        {
            const auto Instance = GetSchemaInstance(SchemaClass);
            if (!Instance || !Instance->HasProperties(GetMandatoryPropertiesForSchema(SchemaClass)))
            {
                return false;
            }
        }
        return true;
    }

    // Per-schema mandatory properties (optional helper)
    TSet<TSubclassOf<ULyokoProperty>> GetMandatoryPropertiesForSchema(TSubclassOf<ULyokoSchema> SchemaClass) const
    {
        if (SchemaClass == USchemaA::StaticClass()) return { UPropertyA::StaticClass() };
        if (SchemaClass == USchemaB::StaticClass()) return { UPropertyB::StaticClass() };
        return {};
    }

    // Default impl helpers:
    // auto* prop = GetProperty<UPropertyA>(USchemaA::StaticClass());
    // bool has = HasProperty(UPropertyA::StaticClass()); // checks all schemas
};
```

---

## Concrete Schema Implementations

### UItemSchema (LyokoBase)

```
UItemSchema : ULyokoSchema
  └── DisplayName: FText
  └── Description: FText
  └── Icon: UTexture2D*
```

**Special notes:**
- The **only** schema with an `Icon` field — items display icons in inventories/UI
- Properties composed by item schemas:
  - `UDurabilityProperty` — required by `AItem`
  - `UAttackCooldownProperty` — required by `AWeaponBase`
  - `UReloadProperty` — required by `ARangedWeapon`
  - `UProjectileProperty` — required by `ARangedWeapon`
  - `UDamageModifierProperty` — required by `ARangedWeapon`
- `AItem::IsItemDefinitionValid()` validates that all mandatory properties are present
- `AWeaponBase::GetMandatoryProperties_Implementation()` extends `AItem`'s requirements by appending `UAttackCooldownProperty`
- `ARangedWeapon::GetMandatoryProperties_Implementation()` extends `AWeaponBase`'s requirements by appending `UReloadProperty`, `UProjectileProperty`, and `UDamageModifierProperty`
- **Interface:** `AItem` / `AWeaponBase` / `ARangedWeapon` implement `ISchemaConsumer` (via `UItemSchema`)

### UProjectileSchema (LyokoWeapons)

```
UProjectileSchema : ULyokoSchema
  └── DisplayName: FText
  └── Description: FText
```

**Special notes:**
- Minimal schema — all projectile-specific data lives in composed properties
- Properties composed by projectile schemas:
  - `UProjectileDamageProperty` — base damage
  - `UProjectileRadiusProperty` — collision radius
  - `UProjectileOnHitBehaviorProperty` — hit behavior
  - `UProjectileImpulseProperty` — impulse force + Master Field
- `AProjectileBase` uses `GetMandatoryProperties()` / `GetOptionalProperties()` for property requirements (overridable in C++ and Blueprint)
- `AProjectileBase` provides convenience accessors: `GetDamage()`, `GetRadius()`, `GetOnHitBehavior()` that safely return defaults if the schema or property is null
- **Interface:** `AProjectileBase` / `AImpulseProjectile` implement `ISchemaConsumer` (via `UProjectileSchema`)

---

## Schema Inheritance Chain

```
UPrimaryDataAsset
  └── ULyokoSchema (base composition system)
       ├── UItemSchema
       │    └── (used by: AItem, AWeaponBase, ARangedWeapon)
       │         └── Properties: UDurabilityProperty, UAttackCooldownProperty,
       │              UReloadProperty, UProjectileProperty, UDamageModifierProperty
       │         └── Interface: ISchemaConsumer
       │
       └── UProjectileSchema
            └── (used by: AProjectileBase, AImpulseProjectile)
                 └── Properties: UProjectileDamageProperty, UProjectileRadiusProperty,
                      UProjectileOnHitBehaviorProperty, UProjectileImpulseProperty
                 └── Interface: ISchemaConsumer
```

## Actor Interface Implementation

### ISchemaConsumer Implementation

Actors implementing `ISchemaConsumer` must provide:

| Method | Implementation |
|--------|----------------|
| `GetSchema()` | Returns the schema member (e.g., `ItemDefinition`, `ProjectileSchema`) |
| `GetMandatoryProperties()` | Returns required property classes (e.g., `{ UDurabilityProperty::StaticClass() }`) |
| `GetOptionalProperties()` | Returns optional property classes (usually `{}`) |
| `IsSchemaValid()` | Checks `Schema != nullptr && Schema->HasProperties(GetMandatoryProperties())` |

### IMultipleSchemaConsumer Implementation

Actors implementing `IMultipleSchemaConsumer` must provide:

| Method | Implementation |
|--------|----------------|
| `GetSchemaList()` | Returns array of schema types used |
| `GetSchemaInstance(SchemaClass)` | Looks up instance in a `TMap<TSubclassOf<ULyokoSchema>, TObjectPtr<ULyokoSchema>>` |
| `HasSchema(SchemaClass)` | Checks if map contains the key |
| `IsSchemaValid()` | Iterates all schemas, validates each has required properties |

The actor stores schema instances in a `TMap<TSubclassOf<ULyokoSchema>, TObjectPtr<ULyokoSchema>>` and populates it via a `RegisterSchema()` method or in `BeginPlay()`.

---

## Summary

The Lyoko Gameplay Data Architecture provides:

1. **Modular composition** — each DataAsset composes only the properties it needs
2. **Single source of truth** — all game data lives in DataAssets, not actors
3. **Actor interface contract** — `ISchemaConsumer` / `IMultipleSchemaConsumer` formalize the actor-schema relationship
4. **Runtime validation** — mandatory properties enforced at assignment and runtime
5. **Editor validation** — automatic checks for key/value consistency and property data validity
6. **Typed access** — `GetProperty<T>()` provides compile-time checked property access
7. **Extensibility** — new property types can be added without modifying any schema
8. **Blueprint support** — all schemas and properties are BlueprintType/Blueprintable

This architecture is designed to scale with the game: adding new entity types only requires a new schema (with DisplayName/Description) and any new property types, with no changes to the base system.
