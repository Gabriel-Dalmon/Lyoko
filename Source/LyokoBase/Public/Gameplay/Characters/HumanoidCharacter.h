// Copyright © 2025 Lyoko - 96 l'Art Cheperdu

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Characters/LyokoCharacter.h"
#include "Gameplay/Characters/MovementCharacter.h"
#include "Gameplay/Combat/CombatActor.h"
#include "Gameplay/Interactor.h"
#include "Gameplay/Pickuper.h"
#include "Gameplay/Dropper.h"
#include "Gameplay/Items/Item.h"
#include "UObject/ScriptInterface.h"
#include "HumanoidCharacter.generated.h"

class IPickupableItem;

/**
 * 
 */
UCLASS(Abstract)
class LYOKOBASE_API AHumanoidCharacter : public ALyokoCharacter, public ICombatActor, public IInteractor, public IPickuper, public IDropper
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Sockets")
	bool bEnableSocketNamesOverride;

	/* Name of the Left Grab Socket */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sockets", meta = (EditCondition = "bEnableSocketNamesOverride"))
	FName LeftHandGripSocketName = TEXT("HandGrip_L");

	/* Name of the Right Grab Socket */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sockets", meta = (EditCondition = "bEnableSocketNamesOverride"))
	FName RightHandGripSocketName = TEXT("HandGrip_R");
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Items")
	TObjectPtr<AItem> MainItem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Items")
	TArray<TScriptInterface<IPickupable>> Pickupables;

public:
	virtual void Pickup_Implementation(const TScriptInterface<IPickupable>& Pickupable);

	virtual void Drop_Implementation(const TScriptInterface<IDroppable>& Droppable);
	
	virtual void OnPickupableInReach_Implementation(const TScriptInterface<IPickupable>& Pickupable) override;

	virtual void OnPickupableOutOfReach_Implementation(const TScriptInterface<IPickupable>& Pickupable) override;

	virtual void Interact_Implementation(EInteractionTypes Type);

	inline virtual FVector GetAttackDirection_Implementation(FVector AttackOrigin) const {
		if (Controller && Controller->Implements<UCombatActor>())
		{
			return ICombatActor::Execute_GetAttackDirection(Controller, AttackOrigin);
		}
		return GetActorRotation().Vector();
	}

	inline virtual EAttackDirectionModes GetAttackDirectionMode_Implementation() const {
		return EAttackDirectionModes::UseCombatActorDirection;
	}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void PrimaryInteract();
	virtual void PrimaryInteract_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SecondaryInteract();
	virtual void SecondaryInteract_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void TernaryInteract();
	virtual void TernaryInteract_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void DropMainItem();
	virtual void DropMainItem_Implementation();

	UFUNCTION(BlueprintCallable)
	FTransform GetGrabTransform(ERelativeTransformSpace TransformSpace) const;
};
