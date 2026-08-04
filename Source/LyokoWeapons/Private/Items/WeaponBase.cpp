// Copyright © 2025-2027 Lyoko - 96 l'Art Cheperdu

#include "Items/WeaponBase.h"
#include "Gameplay/Combat/CombatActor.h"
#include "Gameplay/InteractionTypes.h"

void AWeaponBase::Interact_Implementation(EInteractionTypes Type)
{
    switch (Type)
    {
    case EInteractionTypes::Primary:
        OnPrimaryInteracted();
        break;
    case EInteractionTypes::Secondary:
        OnSecondaryInteracted();
        break;
    default:
        break;
    }
}

void AWeaponBase::OnPrimaryInteracted_Implementation()
{
    Attack();
}

void AWeaponBase::Attack()
{
    if (!IsAttackAvailable()) return;

    //TODO@g: Give the weapon owner an opportunity to override weapon's attack rate. Ex attack rate multiplier. Attackrate of 0?

    if (Owner &&
        Owner->Implements<UCombatActor>() &&
        ICombatActor::Execute_GetAttackDirectionMode(Owner) == EAttackDirectionModes::UseCombatActorDirection
        )
    {
        //TODO@g - Figure out the flow from MainItem interacted to notifying the Owner it should play the attack animation and call the actual Attack in an anim notifier or later.
        const FVector Direction = ICombatActor::Execute_GetAttackDirection(Owner, GetAttackOrigin());
        ICombatActor::Execute_AttackInDirection(Owner, Direction);
        //AttackInDirection(Direction);
    }
    else {
        AttackInDirection(GetDefaultAttackDirection());
    }
}

void AWeaponBase::AttackInDirection_Implementation(const FVector &Direction)
{
    LastAttackTime = GetWorld()->GetTimeSeconds();
    if (!bIsUnbreakable && CurrentDurability > 0)
    {
        --CurrentDurability;
    }
}

bool AWeaponBase::IsAttackAvailable_Implementation() const
{
    auto CurrentTime = GetWorld()->GetTimeSeconds();
    auto LastAttackElapsedTime = CurrentTime - LastAttackTime;

    return LastAttackElapsedTime < GetProperty<UAttackCooldownProperty>()->AttackCooldown;
}

FVector AWeaponBase::GetDefaultAttackDirection_Implementation() const
{
    return GetActorForwardVector();
}

float AWeaponBase::ComputeDamageMultiplier_Implementation() const
{
    return GetInstigatorDamageMultiplier();
}

float AWeaponBase::GetInstigatorDamageMultiplier() const
{
    APawn *AttackInstigator = GetInstigator();
    if (!AttackInstigator || !AttackInstigator->Implements<UCombatActor>())
    {
        return 1.0f;
    }
    return ICombatActor::Execute_GetAttackMultiplier(AttackInstigator);
}
