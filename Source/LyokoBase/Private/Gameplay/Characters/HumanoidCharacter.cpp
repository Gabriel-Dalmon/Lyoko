// Copyright © 2025 Lyoko - 96 l'Art Cheperdu

#include "Gameplay/Characters/HumanoidCharacter.h"
#include "Gameplay/Interactable.h"
#include "Gameplay/Pickupable.h"
#include "Gameplay/Droppable.h"

/**
* @param Pickupable - The Pickupable to pick up
*/
void AHumanoidCharacter::Pickup_Implementation(const TScriptInterface<IPickupable>& Pickupable)
{
	if (!Pickupable)
	{
		return;
	}
	if (MainItem) {
		if (MainItem->Implements<UDroppable>()) { //TODO@g: Probably constrain MainItem to always be IDroppable, IPickupable
			IDropper::Execute_Drop(this, MainItem);
		}
		MainItem = nullptr;
	}
	MainItem = Cast<AItemBase>(Pickupable.GetObject());
	if (!MainItem) 
	{
		return;
	}
	MainItem->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, LeftHandGripSocketName);
	IPickupable::Execute_OnPickedUp(MainItem, this);
}

/**
* @param Droppable - The Droppable to drop
*/
void AHumanoidCharacter::Drop_Implementation(const TScriptInterface<IDroppable>& Droppable)
{
	if (!Droppable)
	{
		return;
	}
	
	IDroppable::Execute_OnDropped(Droppable.GetObject(), this);
}

/**
* @param Pickupable - The Pickupable that came into reach
*/
void AHumanoidCharacter::OnPickupableInReach_Implementation(const TScriptInterface<IPickupable>& Pickupable)
{
	if (!Pickupable)
	{
		return;
	}
	Pickupables.Add(Pickupable);
}

/**
* @param Pickupable - The Pickupable that went out of reach
*/
void AHumanoidCharacter::OnPickupableOutOfReach_Implementation(const TScriptInterface<IPickupable>& Pickupable)
{
	if (!Pickupable)
	{
		return;
	}
	Pickupables.RemoveSwap(Pickupable);
}

/**
* @param Type - Type of interaction
*/
void AHumanoidCharacter::Interact_Implementation(EInteractionTypes Type)
{
	switch (Type) {
	case EInteractionTypes::Primary:
		PrimaryInteract();
		break;
	case EInteractionTypes::Secondary:
		SecondaryInteract();
	case EInteractionTypes::Ternary:
		TernaryInteract();
	}
}

/**
*/
void AHumanoidCharacter::PrimaryInteract_Implementation()
{
	if (MainItem && MainItem->Implements<UInteractable>()) {
		IInteractable::Execute_OnInteracted(MainItem, EInteractionTypes::Primary);
	}
}

/**
*/
void AHumanoidCharacter::SecondaryInteract_Implementation()
{
	// Pickup Pickupable in reach if no empty handed
	if (!MainItem && !Pickupables.IsEmpty())
	{
		auto Pickupable = Pickupables.Last();
		IPickuper::Execute_Pickup(this, Pickupable);
		Pickupables.RemoveSwap(Pickupable);

	}
	else
	{
		//Try to interact with environment (list of IInteractables)
	}
}

/**
*/
void AHumanoidCharacter::TernaryInteract_Implementation()
{
	//Drop MainItem or Swap with another Pickupable in reach
	if (MainItem) {
		if (!Pickupables.IsEmpty()) {
			auto Pickupable = Pickupables.Last();
			IPickuper::Execute_Pickup(this, Pickupable);
			Pickupables.RemoveSwap(Pickupable);
		}
		else {
			DropMainItem();
		}
	}
}

/**
*/
void AHumanoidCharacter::DropMainItem_Implementation()
{
	if (MainItem) {
		checkf(MainItem->Implements<UDroppable>(), TEXT("Tried to drop %s's MainItem without the %s interface."), *GetName(), *UDroppable::StaticClass()->GetName());
		IDropper::Execute_Drop(this, MainItem);
	}
	MainItem = nullptr;
}

/**
* @param TransformSpace - Space in which the transform should be returned
* @return Grab transform subobject
*/
FTransform AHumanoidCharacter::GetGrabTransform(ERelativeTransformSpace TransformSpace) const
{
	USkeletalMeshComponent* MeshComponent = GetMesh();
	FName HandleSocketName = LeftHandGripSocketName;
	if ((MeshComponent && MeshComponent->DoesSocketExist(HandleSocketName)) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: Socket '%s' not found on mesh!"), *AHumanoidCharacter::StaticClass()->GetName(), *HandleSocketName.ToString());
		return FTransform();
	}
	return MeshComponent->GetSocketTransform(HandleSocketName, TransformSpace);
}