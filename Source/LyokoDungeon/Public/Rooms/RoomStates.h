// Copyright © 2025 Lyoko - 96 l'Art Cheperdu

#pragma once

#include "CoreMinimal.h"
#include "RoomStates.generated.h"

UENUM(BlueprintType)
enum class ERoomStates : uint8
{
    NotStarted UMETA(DisplayName = "NotStarted", ToolTip = "(Room is Not Started)"),    
    InProgress UMETA(DisplayName = "InProgress", ToolTip = "(Room is in Progress)"),
    Cleared UMETA(DisplayName = "Cleared", ToolTip = "(Room is Cleared)"),
    Failed UMETA(DisplayName = "Failed", ToolTip = "(Room is Failed)"),
};