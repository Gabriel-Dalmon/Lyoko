// Copyright � 2025 Lyoko - 96 l'Art Cheperdu

#include "Tasks/BTTask_MoveToTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "Kismet/GameplayStatics.h"

UBTTask_MoveToTarget::UBTTask_MoveToTarget()
    : TargetActorKey("TargetActor")
    , AcceptableRadius(64.f)
    , bUpdatePathOnMove(true)
    , bUseCustomTargetKey(false)
    , bTargetValid(false)
{
    NodeName = "Move To Target";
    bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_MoveToTarget::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8* NodeMemory)
{
    AActor* TargetActor = nullptr;
    AAIController &Controller = *OwnerComp.GetAIOwner();

    if (bUseCustomTargetKey)
    {
        TargetActor = Cast<AActor>(Controller.GetBlackboardComponent()->GetValueAsObject(TargetActorKey));
    }
    else
    {
        TargetActor = Cast<AActor>(Controller.GetBlackboardComponent()->GetValueAsObject("TargetActor"));
    }

    if (!TargetActor || !Controller.GetPawn())
    {
        return EBTNodeResult::Failed;
    }

    LastKnownTargetLocation = TargetActor->GetActorLocation();
    bTargetValid = true;

    // Initial navigation request
    UNavigationPath* NavPath =
        UNavigationSystemV1::FindPathToActorSynchronously(
            Controller.GetWorld(),
            Controller.GetPawn()->GetActorLocation(),
            TargetActor,
            AcceptableRadius,
            nullptr
        );
    if (NavPath && NavPath->IsValid())
    {
        Controller.MoveToLocation(LastKnownTargetLocation, AcceptableRadius);
        return EBTNodeResult::InProgress;
    }

    // Try to move as close as possible even if path is blocked
    FNavLocation NavLocation;
    UNavigationSystemV1* NavSys = UNavigationSystemV1::GetNavigationSystem(Controller.GetWorld());
    if (NavSys)
    {
        FNavLocation StartLocation;
        const bool bStartLocationValid = NavSys->ProjectPointToNavigation(Controller.GetPawn()->GetActorLocation(), StartLocation);
        const bool bTargetLocationValid = NavSys->ProjectPointToNavigation(LastKnownTargetLocation, NavLocation);

        if (bStartLocationValid && bTargetLocationValid)
        {
            Controller.MoveToLocation(NavLocation.Location, AcceptableRadius);
            return EBTNodeResult::InProgress;
        }
    }

    return EBTNodeResult::Failed;
}

void UBTTask_MoveToTarget::TickTask(UBehaviorTreeComponent &OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    AAIController *Controller = OwnerComp.GetAIOwner();
    if (!Controller) return;

    if (!bTargetValid)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    // Check if we've reached the target
    AActor* TargetActor = nullptr;
    UBlackboardComponent *Blackboard = OwnerComp.GetBlackboardComponent();


    if (bUseCustomTargetKey)
    {
        TargetActor = Cast<AActor>(Blackboard->GetValueAsObject(TargetActorKey));
    }
    else
    {
        TargetActor = Cast<AActor>(Blackboard->GetValueAsObject("TargetActor"));
    }

    if (!TargetActor)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    // Update target location (for tracking moving targets)
    FVector CurrentTargetLocation = TargetActor->GetActorLocation();
    APawn *ControlledPawn = Controller->GetPawn();
    if (!ControlledPawn)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    if (FVector::DistSquared(ControlledPawn->GetActorLocation(), CurrentTargetLocation) < (AcceptableRadius * AcceptableRadius))
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        return;
    }

    // Update path if needed for tracking
    if (bUpdatePathOnMove && FVector::DistSquared(LastKnownTargetLocation, CurrentTargetLocation) > 100.f)
    {
        LastKnownTargetLocation = CurrentTargetLocation;
        UNavigationPath* NavPath = UNavigationSystemV1::FindPathToActorSynchronously(
            Controller->GetWorld(),
            ControlledPawn->GetActorLocation(),
            TargetActor,
            AcceptableRadius,
            nullptr
        );

        if (NavPath && NavPath->IsValid())
        {
            Controller->MoveToLocation(LastKnownTargetLocation, AcceptableRadius);
        }
    }
}
