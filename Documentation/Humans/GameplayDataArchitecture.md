# Gameplay Data Architecture

## TL;DR

All game data lives in **DataAssets** that compose small, reusable **properties**. Actors reference a schema and query its properties at runtime. The contract is formalized by `ISchemaConsumer` (single schema) or `IMultipleSchemaConsumer` (multiple schemas).

**Core idea:** Composition over inheritance. New data types = new property classes, not new schema subclasses.

---

## The Model

```
Schema (DataAsset)
├── DisplayName / Description (metadata)
└── Properties: TMap<PropertyClass, PropertyInstance>

Property (UObject)
└── One or more fields (float, int, TSubclassOf, etc.)
```

Each schema composes only the properties it needs. `UDurabilityProperty`, `UProjectileDamageProperty`, `UAttackCooldownProperty` — they're all independent. Add a new one and it's immediately available in any schema's editor.

### Key Classes

| Class | Role |
|-------|------|
| `ULyokoSchema` | Base for all game data assets. Holds `Properties` map + validation |
| `ULyokoProperty` | Base for all property types. `EditInlineNew` + `DefaultToInstanced` = inline composition in editor |
| `ISchemaConsumer` | Interface: actor declares what it needs, schema provides it |
| `IMultipleSchemaConsumer` | Same, but for actors using multiple schemas |

---

## How It Works

### 1. Define Data in the Editor

Create a DataAsset → pick a schema → compose properties in the Details panel:

```
UItemSchema "MySword"
├── DisplayName: "Plasma Blade"
├── Icon: Texture2D
└── Properties:
     ├── UDurabilityProperty → { MaximumDurability = 200 }
     ├── UAttackCooldownProperty → { AttackCooldown = 0.5 }
     └── UDamageModifierProperty → { DamageModifier = 1.5 }
```

### 2. Reference from an Actor

```cpp
class AMyWeapon : public AActor, public ISchemaConsumer
{
    TObjectPtr<UItemSchema> ItemDefinition;

    virtual ULyokoSchema* GetSchema_Implementation() const override
    { return ItemDefinition; }

    virtual TSet<TSubclassOf<ULyokoProperty>>
    GetMandatoryProperties_Implementation() const override
    { return { UDurabilityProperty::StaticClass() }; }

    virtual TSet<TSubclassOf<ULyokoProperty>>
    GetOptionalProperties_Implementation() const override
    { return {}; }

    virtual bool IsSchemaValid_Implementation() const override
    {
        if (!ItemDefinition) return false;
        return ItemDefinition->HasProperties(GetMandatoryProperties());
    }
};
```

### 3. Query Properties at Runtime

```cpp
// Default implementations — no boilerplate needed
auto* durability = GetProperty<UDurabilityProperty>();
float maxDur = durability ? durability->MaximumDurability : 0;

bool hasCooldown = HasProperty(UAttackCooldownProperty::StaticClass());
```

---

## The Contract

```
Actor (ISchemaConsumer)              Schema (DataAsset)
─────────────────────                ──────────────────
GetSchema() → ItemDefinition         Properties map
GetMandatoryProperties() → {UDur}    [UDurabilityProperty]
IsSchemaValid() → HasProperties()    → true/false
GetProperty<T>() → .MaximumDurability
```

The actor **declares** what it needs → the schema **provides** it → `IsSchemaValid()` ensures the contract is fulfilled.

---

## Concrete Schemas

### UItemSchema (items/weapons)
| Field | Type | Purpose |
|-------|------|---------|
| `DisplayName` | FText | Display name |
| `Description` | FText | Item description |
| `Icon` | UTexture2D* | Inventory icon |

**Composed properties:** `UDurabilityProperty`, `UAttackCooldownProperty`, `UReloadProperty`, `UProjectileProperty`, `UDamageModifierProperty`

### UProjectileSchema (projectiles)
| Field | Type | Purpose |
|-------|------|---------|
| `DisplayName` | FText | Display name |
| `Description` | FText | Description |

**Composed properties:** `UProjectileDamageProperty`, `UProjectileRadiusProperty`, `UProjectileOnHitBehaviorProperty`, `UProjectileImpulseProperty`

---

## Property Reference

| Property | Field(s) | Default | Used By |
|----------|----------|---------|---------|
| `UDurabilityProperty` | `MaximumDurability: int` | 100 | AItem |
| `UDamageProperty` | *(empty)* | — | — |
| `UAttackCooldownProperty` | `AttackCooldown: float` | 1.0 | AWeaponBase |
| `UDamageModifierProperty` | `DamageModifier: float` | 1.0 | ARangedWeapon |
| `UReloadProperty` | Capacity, BatchSize, Duration, Pattern | — | ARangedWeapon |
| `UProjectileProperty` | `ProjectileClass: TSubclassOf<AProjectileBase>` | — | ARangedWeapon |
| `UProjectileDamageProperty` | `Damage: float` | 10.0 | AProjectileBase |
| `UProjectileRadiusProperty` | `Radius: float` | 5.0 | AProjectileBase |
| `UProjectileOnHitBehaviorProperty` | `OnHitBehavior: EProjectileOnHitBehavior` | Destroy | AProjectileBase |
| `UProjectileImpulseProperty` | `ImpulseStrength: float`, `MasterFieldClass` | 100.0 | AImpulseProjectile |

---

## Creating New Types

### New Property (30 seconds)

```cpp
UCLASS()
class UMyProperty : public ULyokoProperty
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere) float MyValue = 1.0f;
    virtual EDataValidationResult IsDataValid(FDataValidationContext& C) const override
    { return MyValue > 0 ? EDataValidationResult::Valid : EDataValidationResult::Invalid; }
};
```

### New Schema (10 seconds)

```cpp
UCLASS(BlueprintType)
class UMySchema : public ULyokoSchema
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere) FText DisplayName;
    UPROPERTY(EditAnywhere) FText Description;
};
```

### New Actor (50 seconds)

```cpp
UCLASS()
class AMyActor : public AActor, public ISchemaConsumer
{
    GENERATED_BODY()
private:
    TObjectPtr<UMySchema> MySchema;
public:
    virtual ULyokoSchema* GetSchema_Implementation() const override { return MySchema; }
    virtual TSet<TSubclassOf<ULyokoProperty>> GetMandatoryProperties_Implementation() const override
    { return { UMyProperty::StaticClass() }; }
    virtual TSet<TSubclassOf<ULyokoProperty>> GetOptionalProperties_Implementation() const override { return {}; }
    virtual bool IsSchemaValid_Implementation() const override
    { return MySchema && MySchema->HasProperties(GetMandatoryProperties()); }
};
```

---

## Schema Inheritance

```
UPrimaryDataAsset
  └── ULyokoSchema
       ├── UItemSchema → ISchemaConsumer (AItem, AWeaponBase, ARangedWeapon)
       └── UProjectileSchema → ISchemaConsumer (AProjectileBase, AImpulseProjectile)
```

---

## Summary

| Principle | How |
|-----------|-----|
| Composition over inheritance | Properties are composed in a map, not inherited |
| Single source of truth | All data in DataAssets, not actors |
| Actor-schema contract | `ISchemaConsumer` / `IMultipleSchemaConsumer` interfaces |
| Validation | `GetMandatoryProperties()` + `IsSchemaValid()` at runtime |
| Extensibility | New property = new class, zero schema changes |
