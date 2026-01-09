#pragma once

#include "CoreMinimal.h"
#include "ProjectileOnHitBehavior.generated.h"

UENUM(BlueprintType)
enum class EProjectileOnHitBehavior : uint8
{
    Destroy             UMETA(DisplayName="Destroy"),
    Passthrough         UMETA(DisplayName="Passthrough"),
    LetTargetDecide     UMETA(DisplayName="LetTargetDecide (Not implemented, defaults to Destroy)"),
};