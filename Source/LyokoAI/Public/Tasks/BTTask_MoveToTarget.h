// Copyright � 2025 Lyoko - 96 l'Art Cheperdu

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_MoveToTarget.generated.h"

UCLASS(Category = "LyokoAI")
class LYOKOAI_API UBTTask_MoveToTarget : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UBTTask_MoveToTarget();

    // Blackboard key names
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard",
        meta = (EditCondition = "bUseCustomTargetKey", InlineEditConditionToggle))
    FName TargetActorKey;

    // How close to get before succeeding
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement",
        meta = (ClampMin = "0", UIMin = "0", UIMax = "500"))
    float AcceptableRadius;

    // If true, update the move request each tick to track moving targets
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    bool bUpdatePathOnMove;

    // Whether to use a custom target key (false = use default "TargetActor")
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard")
    bool bUseCustomTargetKey;

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) override;
    virtual void TickTask(UBehaviorTreeComponent &OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
    // Current latent move request state
    TWeakObjectPtr<UAIPerceptionComponent> PerceptionComp;
    FVector LastKnownTargetLocation;
    bool bTargetValid;
};
