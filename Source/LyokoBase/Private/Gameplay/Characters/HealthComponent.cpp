// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/Characters/HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    bAutoActivate = true;
    MaxHealth = 100.f;
    CurrentHealth = MaxHealth;
}

void UHealthComponent::TakeDamage(float DamageAmount)
{
    if (DamageAmount < 0 || FMath::IsNearlyZero(DamageAmount)) return;

    const float PreviousHealth = CurrentHealth;
    CurrentHealth = FMath::Clamp(CurrentHealth - DamageAmount, 0.0f, MaxHealth);
    if (CurrentHealth != PreviousHealth)
    {
        OnHealthUpdatedEvent.Broadcast(PreviousHealth, CurrentHealth);

        if (CurrentHealth < 0 || FMath::IsNearlyZero(CurrentHealth))
        {
            OnDeadEvent.Broadcast();
        }
    }
}

void UHealthComponent::Heal(float HealAmount)
{
    if (HealAmount < 0 || FMath::IsNearlyZero(HealAmount)) return;

    const float PreviousHealth = CurrentHealth;
    CurrentHealth = FMath::Clamp(CurrentHealth + HealAmount, 0.0f, MaxHealth);
    if (CurrentHealth != PreviousHealth)
    {
        OnHealthUpdatedEvent.Broadcast(PreviousHealth, CurrentHealth);

        const bool IsAlive = !FMath::IsNearlyZero(CurrentHealth) && CurrentHealth > 0;
        const bool WasDead = FMath::IsNearlyZero(PreviousHealth) || PreviousHealth < 0;
        if (IsAlive && WasDead)
        {
            OnResurectEvent.Broadcast();
        }
    }
}
