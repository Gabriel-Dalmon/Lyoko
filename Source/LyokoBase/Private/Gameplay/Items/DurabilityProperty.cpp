// Copyright © 2025-2027 Lyoko - 96 l'Art Cheperdu


#include "Gameplay/Items/DurabilityProperty.h"

EDataValidationResult UDurabilityProperty::IsDataValid(FDataValidationContext &Context) const
{
    return MaximumDurability > 0.0f ? EDataValidationResult::Valid : EDataValidationResult::Invalid;
}
