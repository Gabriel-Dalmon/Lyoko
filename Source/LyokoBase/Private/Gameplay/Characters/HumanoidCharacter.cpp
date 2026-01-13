// Copyright © 2025 Lyoko - 96 l'Art Cheperdu

#include "Gameplay/Characters/HumanoidCharacter.h"
#include "Gameplay/Interactable.h"
#include "Gameplay/Items/PickupableItem.h"

void AHumanoidCharacter::Move_Implementation(const FVector2D &Direction)
{
	//--------------------------------------------------------------------------
	/** Clamp the vector to length 1 instead of normalizing it to support
	/* small input values as a speed factor. */

	/* Clamp logic from FVector.GetClampedToSize() applied to FVector2D. */
	const double Magnitude = Direction.Size();
	const FVector2D &NormalizedDirection = (Magnitude > UE_SMALL_NUMBER) ?
		(Direction / Magnitude) : Direction.ZeroVector;
	const double ClampedMagnitude = FMath::Clamp(Magnitude, 0, 1);

	const FVector2D &SanitizedDirection = ClampedMagnitude *
		NormalizedDirection;

	//--------------------------------------------------------------------------
	const FVector MovementInput = FVector(SanitizedDirection.X, SanitizedDirection.Y, 0.0f);
		//(GetActorForwardVector() * SanitizedDirection.X) +
		//(GetActorRightVector() * SanitizedDirection.Y);

	AddMovementInput(MovementInput);
}

void AHumanoidCharacter::Pickup_Implementation(const TScriptInterface<IPickupableItem>& Pickupable)
{
	if (!Pickupable)
	{
		return;
	}
	if (MainItem) {
		//drop
	}
	MainItem = Cast<AItemBase>(Pickupable.GetObject());
	if (!MainItem) 
	{
		return;
	}

	//Pickup
	MainItem->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, LeftGrabSocketName);
	IPickupableItem::Execute_OnPickedUp(MainItem, this); // -> Pickupable->AddActorLocalOffset(-Weapon->GetHandleTransform(ERelativeTransformSpace::RTS_Actor).GetLocation()); //Todo
}

void AHumanoidCharacter::OnPickupableInReach_Implementation(const TScriptInterface<IPickupableItem>& Pickupable)
{
	// Notify controller
}

void AHumanoidCharacter::OnPickupableOutOfReach_Implementation(const TScriptInterface<IPickupableItem>& Pickupable)
{
	// Notify controller
}

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

void AHumanoidCharacter::PrimaryInteract_Implementation()
{
	if (MainItem && MainItem->Implements<UInteractable>()) {
		IInteractable::Execute_OnInteracted(MainItem, EInteractionTypes::Primary);
	}
}

void AHumanoidCharacter::SecondaryInteract_Implementation()
{
	//Try to interact with environment (list of IInteractables)
}

void AHumanoidCharacter::TernaryInteract_Implementation()
{
	//Drop MainItem
}

/**
* @param TransformSpace - Space in which the transform should be returned
* @return Grab transform subobject
*/
FTransform AHumanoidCharacter::GetGrabTransform(ERelativeTransformSpace TransformSpace) const
{
	USkeletalMeshComponent* MeshComponent = GetMesh();
	FName HandleSocketName = LeftGrabSocketName;
	if ((MeshComponent && MeshComponent->DoesSocketExist(HandleSocketName)) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: Socket '%s' not found on mesh!"), *AHumanoidCharacter::StaticClass()->GetName(), *HandleSocketName.ToString());
		return FTransform();
	}
	return MeshComponent->GetSocketTransform(HandleSocketName, TransformSpace);
}