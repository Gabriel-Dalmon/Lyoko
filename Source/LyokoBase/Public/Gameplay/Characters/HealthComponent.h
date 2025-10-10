// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthUpdatedEvent, float, PreviousHealth, float, CurrentHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeadEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnResurectEvent);

UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LYOKOBASE_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health")
	float MaxHealth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health")
	float CurrentHealth;

	UPROPERTY(BlueprintAssignable, Category = "Health")
	FOnHealthUpdatedEvent OnHealthUpdatedEvent;

	UPROPERTY(BlueprintAssignable, Category = "Health")
	FOnDeadEvent OnDeadEvent;

	UPROPERTY(BlueprintAssignable, Category = "Health")
	FOnResurectEvent OnResurectEvent;

public:
	UHealthComponent();

public:
	UFUNCTION(BlueprintCallable, Category = "Health")
	void TakeDamage(float DamageAmount);

	UFUNCTION(BlueprintCallable, Category = "Health")
	void Heal(float HealAmount);
};