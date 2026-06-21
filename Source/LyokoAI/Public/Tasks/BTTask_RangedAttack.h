// Copyright � 2025 Lyoko - 96 l'Art Cheperdu

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_RangedAttack.generated.h"

class AWeaponBase;
class AHumanoidCharacter;

UCLASS(Category = "LyokoAI")
class LYOKOAI_API UBTTask_RangedAttack : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UBTTask_RangedAttack();

    // Blackboard key names
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard",
        meta = (EditCondition = "bUseCustomWeaponKey", InlineEditConditionToggle))
    FName WeaponKey;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard",
        meta = (EditCondition = "bUseCustomTargetKey", InlineEditConditionToggle))
    FName TargetActorKey;

    // Whether to use custom weapon key (false = use default "Weapon")
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard")
    bool bUseCustomWeaponKey;

    // Whether to use custom target key (false = use default "TargetActor")
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard")
    bool bUseCustomTargetKey;

    // Optional: use the AI's possessed pawn as the weapon source if weapon key is empty
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fallback")
    bool bFallbackToPawnLocation;

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8* NodeMemory) override;
};
