#pragma once

#include "CoreMinimal.h"
#include "EAIStates.generated.h"

UENUM(BlueprintType)
enum class EAIStates : uint8
{
    Idle,
    Patrol,
    Inspect,
    Chase,
    Attack
};