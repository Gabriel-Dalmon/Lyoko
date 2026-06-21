// Copyright � 2025 Lyoko - 96 l'Art Cheperdu

#include "Tasks/BTTask_RangedAttack.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "LyokoWeapons/Public/Items/WeaponBase.h"
#include "LyokoBase/Public/Gameplay/Combat/CombatActor.h"
#include "LyokoBase/Public/Gameplay/Characters/HumanoidCharacter.h"
#include "LyokoBase/Public/Gameplay/Characters/LyokoCharacter.h"
#include "Kismet/GameplayStatics.h"

UBTTask_RangedAttack::UBTTask_RangedAttack()
    : WeaponKey("Weapon")
    , TargetActorKey("TargetActor")
    , bUseCustomWeaponKey(false)
    , bUseCustomTargetKey(false)
    , bFallbackToPawnLocation(true)
{
    NodeName = "Ranged Attack";
}

EBTNodeResult::Type UBTTask_RangedAttack::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8* NodeMemory)
{
    // 1. Retrieve the weapon from the blackboard
    AWeaponBase* Weapon = nullptr;
    AAIController &Controller = *OwnerComp.GetAIOwner();

    if (bUseCustomWeaponKey)
    {
        Weapon = Cast<AWeaponBase>(Controller.GetBlackboardComponent()->GetValueAsObject(WeaponKey));
    }
    else
    {
        Weapon = Cast<AWeaponBase>(Controller.GetBlackboardComponent()->GetValueAsObject("Weapon"));
    }

    // 2. Retrieve the target actor from the blackboard
    AActor* TargetActor = nullptr;

    if (bUseCustomTargetKey)
    {
        TargetActor = Cast<AActor>(Controller.GetBlackboardComponent()->GetValueAsObject(TargetActorKey));
    }
    else
    {
        TargetActor = Cast<AActor>(Controller.GetBlackboardComponent()->GetValueAsObject("TargetActor"));
    }

    if (!TargetActor)
    {
        UE_LOG(LogTemp, Warning, TEXT("BTTask_RangedAttack: No target actor in blackboard."));
        return EBTNodeResult::Failed;
    }

    // Get the attack origin: use weapon location if available, otherwise fall back to pawn
    FVector AttackOrigin;
    bool bHasWeapon = (Weapon != nullptr);

    if (bHasWeapon)
    {
        AttackOrigin = Weapon->GetAttackOrigin();
    }
    else if (bFallbackToPawnLocation && Controller.GetPawn())
    {
        AttackOrigin = Controller.GetPawn()->GetActorLocation();
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("BTTask_RangedAttack: No weapon and no fallback pawn available."));
        return EBTNodeResult::Failed;
    }

    // 3. Compute the direction from weapon/pawn to the target
    FVector Direction = (TargetActor->GetActorLocation() - AttackOrigin);
    Direction.Z = 0.f; // Flatten to horizontal plane for top-down aiming
    Direction = Direction.GetSafeNormal();

    if (Direction.IsNearlyZero())
    {
        UE_LOG(LogTemp, Warning, TEXT("BTTask_RangedAttack: Direction to target is zero. Target is at the same position."));
        return EBTNodeResult::Failed;
    }

    // 4. Use the combat system to attack
    if (bHasWeapon)
    {
        // Call AttackInDirection on the weapon directly
        Weapon->AttackInDirection(Direction);
        UE_LOG(LogTemp, Log, TEXT("BTTask_RangedAttack: Weapon attacked target at direction %s"), *Direction.ToString());
    }
    else
    {
        // Fallback: check if the pawn implements ICombatActor and use its direction
        ALyokoCharacter* PawnAsCharacter = Cast<ALyokoCharacter>(Controller.GetPawn());
        if (PawnAsCharacter && PawnAsCharacter->Implements<UCombatActor>())
        {
            FVector CombatDirection = ICombatActor::Execute_GetAttackDirection(PawnAsCharacter, AttackOrigin);
            CombatDirection.Z = 0.f;
            CombatDirection = CombatDirection.GetSafeNormal();

            // If the combat actor's direction is valid, use it; otherwise use the computed direction
            if (!CombatDirection.IsNearlyZero())
            {
                Direction = CombatDirection;
            }
        }

        // If the pawn is a HumanoidCharacter, it has a MainItem which could be a weapon
        AHumanoidCharacter* Humanoid = Cast<AHumanoidCharacter>(Controller.GetPawn());
        if (Humanoid && Humanoid->MainItem)
        {
            AWeaponBase* EquippedWeapon = Cast<AWeaponBase>(Humanoid->MainItem);
            if (EquippedWeapon)
            {
                EquippedWeapon->AttackInDirection(Direction);
                UE_LOG(LogTemp, Log, TEXT("BTTask_RangedAttack: Equipped MainItem weapon attacked target at direction %s"), *Direction.ToString());
            }
        }
    }

    return EBTNodeResult::Succeeded;
}
