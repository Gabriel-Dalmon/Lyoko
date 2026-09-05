// Copyright © 2025-2027 Lyoko - 96 l'Art Cheperdu
//     __                __       
//    / /   __  ______  / /______ 
//   / /   / / / / __ \/ //_/ __ \
//  / /___/ /_/ / /_/ / ,< / /_/ /
// /_____/\__, /\____/_/|_|\____/ 
//       /____/                   

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Items/ItemBase.h"
#include "Gameplay/Pickupable.h"
#include "Gameplay/Droppable.h"
#include "GameplayTagContainer.h"
#include "Gameplay/Items/ItemSchema.h"
#include "Gameplay/Items/DurabilityProperty.h"
#include  "Model/SchemaConsumer.h"
#include "Item.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDurabilityChanged, int, DurabilityDifference, int, NewDurability);

/**
 * 
 */
UCLASS()
class LYOKOBASE_API AItem : public AItemBase, public IPickupable, public IDroppable, public ISchemaConsumer
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsUnbreakable = false;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FGameplayTagContainer ItemTags;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int CurrentDurability = 0;

public:
    UPROPERTY(BlueprintAssignable, Category = "Gameplay|Item")
    FOnDurabilityChanged OnDurabilityChangedEvent;

private:
    UPROPERTY(EditDefaultsOnly, Category = "Item", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UItemSchema> ItemSchema;

public:
#if WITH_EDITOR
    virtual void PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent) override;
#endif

public:
    UFUNCTION(BlueprintCallable, Category = "Gameplay|Item")
    inline UItemSchema *GetItemSchema() const { return ItemSchema; }

public:
    ULyokoSchema *GetSchema_Implementation() const override { return ItemSchema; }

private:
    bool IsItemDefinitionValid() const;

public:
    inline virtual TSet<TSubclassOf<ULyokoProperty>> GetMandatoryProperties_Implementation() const { return { UDurabilityProperty::StaticClass() }; }

protected:
    UPROPERTY(EditAnywhere, Category = "Sockets")
    bool bEnableSocketNamesOverride;

    /* Name of the Handle Socket */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sockets", meta = (EditCondition = "bEnableSocketNamesOverride"))
    FName HandleSocketName = TEXT("Handle");

public:
    FTransform GetHandleTransform(ERelativeTransformSpace TransformSpace = RTS_World) const;

public:
    virtual void OnPickedUp_Implementation(const TScriptInterface<IPickuper>& Pickuper);

    virtual TScriptInterface<IPickuper> GetCarrier_Implementation() const;

    virtual void OnDropped_Implementation(const TScriptInterface<IDropper>& Dropper);
};
