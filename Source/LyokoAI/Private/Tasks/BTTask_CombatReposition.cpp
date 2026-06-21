// Copyright � 2025 Lyoko - 96 l'Art Cheperdu

#include "Tasks/BTTask_CombatReposition.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"

UBTTask_CombatReposition::UBTTask_CombatReposition()
    : TargetActorKey("TargetActor")
    , MinDistance(200.f)
    , MaxDistance(600.f)
    , bUseCustomTargetKey(false)
    , bAddRandomOffset(true)
    , MaxRandomOffset(100.f)
{
    NodeName = "Combat Reposition";
}

EBTNodeResult::Type UBTTask_CombatReposition::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8* NodeMemory)
{
    AActor* TargetActor = nullptr;

    AAIController *Controller = OwnerComp.GetAIOwner();
    if (!Controller) return EBTNodeResult::Failed;

    UBlackboardComponent *Blackboard = OwnerComp.GetBlackboardComponent();
    if (!Blackboard) return EBTNodeResult::Failed;

    if (bUseCustomTargetKey)
    {
        TargetActor = Cast<AActor>(Blackboard->GetValueAsObject(TargetActorKey));
    }
    else
    {
        TargetActor = Cast<AActor>(Blackboard->GetValueAsObject("TargetActor"));
    }

    APawn *Pawn = Controller->GetPawn();
    if (!TargetActor || !Pawn)
    {
        return EBTNodeResult::Failed;
    }

    FVector CurrentLocation = Pawn->GetActorLocation();
    FVector RepositionLocation = ComputeRepositionLocation(TargetActor, CurrentLocation);

    // Clamp to valid navigation area
    FNavLocation NavLocation;
    UWorld *World = Controller->GetWorld();
    UNavigationSystemV1* NavSys = UNavigationSystemV1::GetNavigationSystem(World);
    if (NavSys)
    {
        FNavLocation CurrentNavLocation;
        const bool bCurrentLocationValid = NavSys->ProjectPointToNavigation(CurrentLocation, CurrentNavLocation);
        const bool bRepositionLocationValid = NavSys->ProjectPointToNavigation(RepositionLocation, NavLocation);
        
        if (!bRepositionLocationValid || !bCurrentLocationValid)
        {
            NavLocation = CurrentNavLocation;
        }
    }
    else
    {
        NavLocation.Location = RepositionLocation;
    }

    Controller->MoveToLocation(NavLocation.Location, 50.f);
    return EBTNodeResult::InProgress;
}

FVector UBTTask_CombatReposition::ComputeRepositionLocation(AActor* Target, FVector CurrentLocation) const
{
    FVector ToTarget = Target->GetActorLocation() - CurrentLocation;
    ToTarget.Z = 0.f;
    float CurrentDist = ToTarget.Size();

    FVector DesiredLocation;

    if (CurrentDist < MinDistance)
    {
        // Too close: move away from target
        if (ToTarget.IsNearlyZero())
        {
            // Same position as target, pick arbitrary direction
            ToTarget = FVector::ForwardVector;
        }
        else
        {
            ToTarget = -ToTarget.GetSafeNormal();
        }
        DesiredLocation = Target->GetActorLocation() + (ToTarget * MinDistance);
    }
    else if (CurrentDist > MaxDistance)
    {
        // Too far: move closer to target
        if (ToTarget.IsNearlyZero())
        {
            ToTarget = FVector::ForwardVector;
        }
        else
        {
            ToTarget = ToTarget.GetSafeNormal();
        }
        DesiredLocation = Target->GetActorLocation() + (ToTarget * MaxDistance);
    }
    else
    {
        // Within range: try to find a flanking position
        FVector ToTargetNormalized = ToTarget.GetSafeNormal();
        // Perpendicular vector for flanking (rotate 90 degrees on YZ plane)
        FVector FlankDirection = FVector(-ToTargetNormalized.Y, ToTargetNormalized.X, 0.f);

        // Alternate between left and right flank based on time
        float Time = FPlatformTime::Seconds();
        if (FMath::Fmod(Time, 2.f) < 1.f)
        {
            FlankDirection = -FlankDirection;
        }

        DesiredLocation = Target->GetActorLocation() + (ToTargetNormalized * ((MinDistance + MaxDistance) * 0.5f))
            + (FlankDirection * ((MaxDistance - MinDistance) * 0.3f));
    }

    // Add random offset for unpredictability
    if (bAddRandomOffset && MaxRandomOffset > 0.f)
    {
        float RandomAngle = FMath::FRandRange(-MaxRandomOffset, MaxRandomOffset);
        float RandomElevation = FMath::FRandRange(-MaxRandomOffset, MaxRandomOffset);
        DesiredLocation.X += FMath::Cos(RandomAngle) * MaxRandomOffset;
        DesiredLocation.Y += FMath::Sin(RandomAngle) * MaxRandomOffset;
        DesiredLocation.Z += RandomElevation;
    }

    return DesiredLocation;
}
