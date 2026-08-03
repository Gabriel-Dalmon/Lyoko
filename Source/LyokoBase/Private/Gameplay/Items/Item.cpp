// Copyright © 2025-2027 Lyoko - 96 l'Art Cheperdu

//----------------------------------------------------------------------------------------------------------------------
#include "Gameplay/Items/Item.h"
#include "Gameplay/Items/DroppedItemContainer.h"
#include "Gameplay/Pickuper.h"
#include "Gameplay/Dropper.h"

#if WITH_EDITOR
//----------------------------------------------------------------------------------------------------------------------.
void AItem::PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    if (PropertyChangedEvent.Property && PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(AItem, ItemDefinition))
    {
        if (!IsItemDefinitionValid())
        {
            FMessageLog("PIE").Error(FText::Format(
                NSLOCTEXT("LyokoBase", "InvalidItemDefinition", "{0}: Invalid ItemDefinition!"),
                FText::FromString(GetName())));
        }
    }
}
#endif

//----------------------------------------------------------------------------------------------------------------------
bool AItem::IsItemDefinitionValid() const
{
    if (!ItemDefinition)
    {
        UE_LOG(LogTemp, Error, TEXT("%s: ItemDefinition is not set!"), *AItem::StaticClass()->GetName());
        return false;
    }
    const auto MandatoryProperties = GetMandatoryProperties();
    if (!ItemDefinition->HasProperties(MandatoryProperties))
    {
        UE_LOG(LogTemp, Error, TEXT("%s: ItemDefinition is missing mandatory properties!"), *AItem::StaticClass()->GetName());
        return false;
    }
    return true;
}

/**
* Get the handle transform subobject
* @param TransformSpace - Space in which the transform should be returned
* @return Handle transform subobject
*/
FTransform AItem::GetHandleTransform(ERelativeTransformSpace TransformSpace) const
{
    checkf(MeshComponent && MeshComponent->DoesSocketExist(HandleSocketName),
        TEXT("%s: Socket '%s' not found on mesh!"), *AItem::StaticClass()->GetName(), *HandleSocketName.ToString());

    return MeshComponent->GetSocketTransform(HandleSocketName, TransformSpace);
}

/**
* @param Pickuper - The Pickuper picking up this weapon
*/
void AItem::OnPickedUp_Implementation(const TScriptInterface<IPickuper>& Pickuper)
{
    if (!Pickuper)
    {
        return;
    }
    auto PickuperActor = Cast<AActor>(Pickuper.GetObject());
    checkf(PickuperActor,
        TEXT("%s: Pickuper is not an Actor!"), *AItem::StaticClass()->GetName());
    SetOwner(PickuperActor);
    const auto PickuperInstigator = PickuperActor->GetInstigator();
    SetInstigator(PickuperInstigator ? PickuperInstigator : Cast<APawn>(PickuperActor)); //TODO@g: Review Instigator/Owner approach
    AddActorLocalOffset(GetHandleTransform(ERelativeTransformSpace::RTS_Actor).GetLocation());
}

TScriptInterface<IPickuper> AItem::GetCarrier_Implementation() const
{
    return Owner;
}

/**
* @param Dropper - The Dropper dropping this weapon
*/
void AItem::OnDropped_Implementation(const TScriptInterface<IDropper>& Dropper)
{
    auto DropperObject = Dropper.GetObject();
    checkf(DropperObject, TEXT("%s: Dropper is not a valid object!"), *AItem::StaticClass()->GetName());
    SetOwner(nullptr);
    SetInstigator(nullptr);

    //TODO@g: Get the DroppedItemContainer from a pool instead of spawning a new one each time.
    const FTransform DropTransform = IDropper::Execute_GetDropTransform(DropperObject);
    ADroppedItemContainer* DroppedItemContainer = GetWorld()->SpawnActor<ADroppedItemContainer>(ADroppedItemContainer::StaticClass(), DropTransform);

    IPickuper::Execute_Pickup(DroppedItemContainer, this);
}
