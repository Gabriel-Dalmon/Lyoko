// Copyright � 2025 Lyoko - 96 l'Art Cheperdu

#pragma once

#include "CoreMinimal.h"
#include "InteractionTypes.generated.h"

UENUM(BlueprintType)
enum class EInteractionTypes : uint8
{
    Primary UMETA(DisplayName = "Primary", ToolTip = "(Attack � Left Click | RT)"),
    Secondary UMETA(DisplayName = "Secondary", ToolTip="(Interaction with Environment � E | X)"),
    Ternary UMETA(DisplayName = "Ternary", ToolTip="(Drop Weapon � G | Y)")
};