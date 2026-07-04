// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/LyokoAIControllerBase.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

void ALyokoAIControllerBase::OnPossess(APawn *InPawn)
{
    Super::OnPossess(InPawn);


    if (!BehaviorTreeAsset)
    {
        UE_LOG(LogTemp, Error, TEXT("[%s]: BehaviorTreeAsset is null in %s."), *GetClass()->GetName(), *GetName());
        return;
    }
    UBlackboardComponent *BlackboardComp;
    if (!UseBlackboard(BehaviorTreeAsset->BlackboardAsset, BlackboardComp))
    {
        UE_LOG(LogTemp, Error, TEXT("[%s]: Failed ,to initialize blackboard in %s."), *GetClass()->GetName(), *GetName());
        return;
    }
    BlackboardComponent = BlackboardComp;

    const FBlackboard::FKey StateKeyID =
        Blackboard->GetKeyID(BehaviorStateKey);

    if (StateKeyID != FBlackboard::InvalidKey)
    {
        Blackboard->RegisterObserver(
            StateKeyID,
            this,
            FOnBlackboardChangeNotification::CreateUObject(
                this,
                &ALyokoAIControllerBase::InternalOnBehaviourStateChanged));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("[%s]: State key not found in blackboard in %s."), *GetClass()->GetName(), *GetName());
        return;
    }


    if (!RunBehaviorTree(BehaviorTreeAsset))
    {
        UE_LOG(LogTemp, Error, TEXT("[%s]: Failed to run behavior tree in %s."), *GetClass()->GetName(), *GetName());
        return;
    }
}

void ALyokoAIControllerBase::OnBehaviourStateChanged_Implementation(FName ChangedKeyName)
{
    UE_LOG(LogTemp, Log, TEXT("[%s]: Behavior state changed to %s in %s."), *GetClass()->GetName(), *ChangedKeyName.ToString(), *GetName());
}

EBlackboardNotificationResult ALyokoAIControllerBase::InternalOnBehaviourStateChanged(const UBlackboardComponent &BlackboardComp, FBlackboard::FKey ChangedKeyID)
{
    const FName KeyName = BlackboardComponent->GetKeyName(ChangedKeyID);

    OnBehaviourStateChanged(KeyName);

    return EBlackboardNotificationResult::ContinueObserving;
}
