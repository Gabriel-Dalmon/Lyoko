// Copyright © 2025 Lyoko - 96 l'Art Cheperdu

#pragma once

#include "CoreMinimal.h"
#include "AttackDirectionModes.generated.h"

UENUM(BlueprintType)
enum class EAttackDirectionModes : uint8
{
    UseWeaponDirection UMETA(DisplayName = "Use Weapon Direction"),
    UseCombatActorDirection UMETA(DisplayName = "Use ICombatActor Direction", ToolTip = "Use the direction retrieved through the ICombatActor.GetAttackDirection")
};