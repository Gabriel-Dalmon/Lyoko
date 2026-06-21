// Copyright � 2025 Lyoko - 96 l'Art Cheperdu

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_CombatReposition.generated.h"

UCLASS(Category = "LyokoAI")
class LYOKOAI_API UBTTask_CombatReposition : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UBTTask_CombatReposition();

    // Blackboard key name
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard",
        meta = (EditCondition = "bUseCustomTargetKey", InlineEditConditionToggle))
    FName TargetActorKey;

    // Minimum combat distance from target
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Distance",
        meta = (ClampMin = "0", UIMin = "0"))
    float MinDistance;

    // Maximum combat distance from target
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Distance",
        meta = (ClampMin = "0", UIMin = "0"))
    float MaxDistance;

    // Whether to use a custom target key (false = use default "TargetActor")
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard")
    bool bUseCustomTargetKey;

    // If true, add a random offset to the reposition point to avoid predictable patterns
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Distance")
    bool bAddRandomOffset;

    // Maximum random offset distance (used when bAddRandomOffset is true)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Distance",
        meta = (EditCondition = "bAddRandomOffset", ClampMin = "0", UIMin = "0"))
    float MaxRandomOffset;

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8* NodeMemory) override;

protected:
    FVector ComputeRepositionLocation(AActor* Target, FVector CurrentLocation) const;
};
