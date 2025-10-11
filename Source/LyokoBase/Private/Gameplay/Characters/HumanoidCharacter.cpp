// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Characters/HumanoidCharacter.h"

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
