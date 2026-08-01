// Copyright � 2025 Lyoko - 96 l'Art Cheperdu


#include "Gameplay/Pickupable.h"
#include "Gameplay/Pickuper.h"

bool IPickupable::IsCarried_Implementation() const
{
    const auto ThisObject = const_cast<UObject*>(Cast<UObject>(this));
    checkf(ThisObject != nullptr, TEXT("IsCarried called on Pickupable that is not a UObject."));
    return IPickupable::Execute_GetCarrier(ThisObject).GetObject() != nullptr;
}

bool IPickupable::CanBeStolen_Implementation(const TScriptInterface<IPickuper>& Stealer) const
{
    const auto ThisObject = const_cast<UObject*>(Cast<UObject>(this));
    checkf(ThisObject != nullptr, TEXT("CanBeStolen called on Pickupable that is not a UObject."));
    if (!IPickupable::Execute_IsCarried(ThisObject)) [[unlikely]] //expected to call IsCarried first
    {
        return false;
    }
    auto CarrierObject = IPickupable::Execute_GetCarrier(ThisObject).GetObject();
    return IPickuper::Execute_CanBeStolen(CarrierObject, ThisObject, Stealer.GetObject());
}

void IPickupable::OnStolen_Implementation(const TScriptInterface<IPickuper>& Stealer)
{
    const auto ThisObject = const_cast<UObject*>(Cast<UObject>(this));
    checkf(ThisObject != nullptr, TEXT("OnStolen called on Pickupable that is not a UObject."));
    checkf(IPickupable::Execute_IsCarried(ThisObject) && IPickupable::Execute_CanBeStolen(ThisObject, Stealer), TEXT("OnStolen called on Pickupable in non stealable state (not carried or cannot be stolen)."));
    
    auto CarrierObject = IPickupable::Execute_GetCarrier(ThisObject).GetObject();
    IPickuper::Execute_OnStolen(CarrierObject, Cast<UObject>(this), Stealer);
}
