// Copyright � 2025-2027 Lyoko - 96 l'Art Cheperdu

#include "Controllers/TopDownPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Gameplay/Characters/LyokoCharacterBase.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "Gameplay/Characters/HealthComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Core/LyokoGameModeBase.h"
#include "Gameplay/Interactor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Gameplay/Characters/MovementCharacter.h"
#include "InputDevices/LyokoInputDeviceSubsystem.h"

//----------------------------------------------------------------------------------------------------------------------
void ATopDownPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    BindInputMapping(MovementInputMapping);
    BindInputMapping(LookInputMapping);
    BindInputMapping(InteractionsInputMapping);

    BindInputAction(MovementInputAction, &ATopDownPlayerController::OnMove);
    BindInputAction(DashInputAction, &ATopDownPlayerController::OnDash);
    BindInputAction(LookInputAction, &ATopDownPlayerController::OnLook);
    BindInputAction(PrimaryInteractInputAction, &ATopDownPlayerController::OnPrimaryInteract);
    BindInputAction(SecondaryInteractInputAction, &ATopDownPlayerController::OnSecondaryInteract);
    BindInputAction(TernaryInteractInputAction, &ATopDownPlayerController::OnTernaryInteract);
}

//----------------------------------------------------------------------------------------------------------------------
void ATopDownPlayerController::OnPossess(APawn *InPawn)
{
    Super::OnPossess(InPawn);
    if (InPawn == nullptr) return;

    SetActorTickEnabled(true);
    RespawnPoint = InPawn->GetActorTransform();
    SetControlRotationToCamera(*InPawn);

    // Get the component ref from the interface or cast directly and move to PlayerControllerBase
    if (UHealthComponent *HealthComponent = InPawn->FindComponentByClass<UHealthComponent>())
    {
        HealthComponent->OnDeadEvent.AddDynamic(this, &ATopDownPlayerController::OnPossessedPawnDead);
    }
}

//----------------------------------------------------------------------------------------------------------------------
void ATopDownPlayerController::OnUnPossess()
{
    Super::OnUnPossess();

    SetActorTickEnabled(false);
}

//----------------------------------------------------------------------------------------------------------------------
void ATopDownPlayerController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (bLookAtCursor && bEnableMouseLook_TO_REVIEW)
    {
        LookAtCursor();
    }
}

//----------------------------------------------------------------------------------------------------------------------
/** 
 * @brief Called by LyokoGameModeBase at the end of the RestartPlayer method.
 * The possessed pawn is expected to use the Control Rotation. 
 * It is computed based on the camera direction to ensure topdown movement to be aligned with the view.
 * Part of the RestartPlayer logic overrides the control rotation after OnPossess has been called. This method is used as
 * hook when Control Rotation is no longer overriden by Unreal internal logic.
 */
void ATopDownPlayerController::OnPlayerRestarted_Implementation()
{
    const APawn *PossessedPawn = GetPawn();
    if (PossessedPawn == nullptr) return;

    SetControlRotationToCamera(*PossessedPawn);
}

//----------------------------------------------------------------------------------------------------------------------
void ATopDownPlayerController::OnPossessedPawnDead_Implementation()
{
    //ALyokoGameModeBase *GameMode = Cast<ALyokoGameModeBase>(GetWorld()->GetAuthGameMode());
    //if (GameMode == nullptr) return;
    //GameMode->Respawn(this);
}

#pragma region Movement
//----------------------------------------------------------------------------------------------------------------------
void ATopDownPlayerController::OnMove_Implementation(const FInputActionValue &Value)
{
    APawn *PossessedPawn = GetPawn();
    const bool bIsMovementCharacter = PossessedPawn->GetClass()->ImplementsInterface(UMovementCharacter::StaticClass());
    if (PossessedPawn == nullptr || bIsMovementCharacter == false) return;

    const FVector2D Input = Value.Get<FVector2D>();
    const float Yaw = GetControlRotation().Yaw;

    const FVector2D RotatedInput = Input.GetRotated(Yaw);

    IMovementCharacter::Execute_Move(PossessedPawn, RotatedInput);
}

//----------------------------------------------------------------------------------------------------------------------
void ATopDownPlayerController::OnDash_Implementation(const FInputActionValue &Value)
{
    APawn *PossessedPawn = GetPawn();
    const bool bIsMovementCharacter = PossessedPawn->GetClass()->ImplementsInterface(UMovementCharacter::StaticClass());
    if (PossessedPawn == nullptr || bIsMovementCharacter == false) return;
    FVector Direction = IMovementCharacter::Execute_GetMovementVelocity(PossessedPawn).GetSafeNormal();
    IMovementCharacter::Execute_Dash(PossessedPawn, Direction);
}

//----------------------------------------------------------------------------------------------------------------------
void ATopDownPlayerController::SetControlRotationToCamera(const APawn &NewPawn)
{
    if (UCameraComponent *CameraComponent = NewPawn.FindComponentByClass<UCameraComponent>())
    {
        FRotator CameraRotation = CameraComponent->GetComponentRotation();
        CameraRotation.Pitch = 0.0f;
        CameraRotation.Roll = 0.0f;
        SetControlRotation(CameraRotation);
    }
}
#pragma endregion Movement

#pragma region Look
//----------------------------------------------------------------------------------------------------------------------
void ATopDownPlayerController::OnLook_Implementation(const FInputActionValue &Value)
{
    APawn *PossessedPawn = GetPawn();
    const bool bIsMovementCharacter = PossessedPawn->GetClass()->ImplementsInterface(UMovementCharacter::StaticClass());
    if (PossessedPawn == nullptr || bIsMovementCharacter == false) return;
    const FVector2D Input = Value.Get<FVector2D>();

    IMovementCharacter::Execute_Look(PossessedPawn, FVector(-Input.X, Input.Y, 0.f));
}

//----------------------------------------------------------------------------------------------------------------------
void ATopDownPlayerController::LookAtCursor()
{
    FHitResult OutHit;
    const bool bHit = GetHitResultUnderCursor(ECC_GameTraceChannel1, true, OutHit);
    if (bHit == false) return;

    APawn *PossessedPawn = GetPawn();
    const bool bIsMovementCharacter = PossessedPawn->GetClass()->ImplementsInterface(UMovementCharacter::StaticClass());
    if (PossessedPawn == nullptr || bIsMovementCharacter == false) return;

    FVector LookDirection = OutHit.Location - PossessedPawn->GetActorLocation();
    LookDirection.Z = 0.0f;
    IMovementCharacter::Execute_Look(PossessedPawn, LookDirection);
}
#pragma endregion Look

#pragma region Interactions
//----------------------------------------------------------------------------------------------------------------------
void ATopDownPlayerController::OnPrimaryInteract_Implementation(const FInputActionValue& Value)
{
    ForwardInteractionToPawn(EInteractionTypes::Primary);
}

//----------------------------------------------------------------------------------------------------------------------
void ATopDownPlayerController::OnSecondaryInteract_Implementation(const FInputActionValue& Value)
{
    ForwardInteractionToPawn(EInteractionTypes::Secondary);
}

//----------------------------------------------------------------------------------------------------------------------
void ATopDownPlayerController::OnTernaryInteract_Implementation(const FInputActionValue& Value)
{
    ForwardInteractionToPawn(EInteractionTypes::Ternary);
}

//----------------------------------------------------------------------------------------------------------------------
void ATopDownPlayerController::ForwardInteractionToPawn(EInteractionTypes Type) const
{
    APawn* PossessedPawn = GetPawn();

    if (PossessedPawn && PossessedPawn->Implements<UInteractor>())
    {
        IInteractor::Execute_Interact(PossessedPawn, Type);
    }
}
#pragma endregion Interactions

#pragma region Combat
//----------------------------------------------------------------------------------------------------------------------
FVector ATopDownPlayerController::GetAttackDirection_Implementation(FVector AttackOrigin) const
{
    FVector Direction;
    if (bLookAtCursor) {
        FVector RayStart, RayDirection;
        DeprojectMousePositionToWorld(RayStart, RayDirection);
        FVector RayHitLocation = FMath::LinePlaneIntersection(RayStart, RayStart + RayDirection * 10000.0f, FVector(0.0f, 0.0f, AttackOrigin.Z), FVector::UpVector);
        Direction = RayHitLocation - AttackOrigin;
    }
    else if (APawn *PossessedPawn = GetPawn(); PossessedPawn) {
        Direction = PossessedPawn->GetActorRotation().Vector();
    }

    Direction.Z = 0.0f;
    return Direction.GetSafeNormal();
}
#pragma endregion Combat

#pragma region Pause
//----------------------------------------------------------------------------------------------------------------------
void ATopDownPlayerController::OnGamePaused_Implementation()
{
    Super::OnGamePaused_Implementation();
    //TODO@g: Show pause menu
    SetMouseCursorWidget(EMouseCursor::Default, nullptr);
    UnbindInputMapping(MovementInputMapping);
    UnbindInputMapping(LookInputMapping);
    UnbindInputMapping(InteractionsInputMapping);
}

//----------------------------------------------------------------------------------------------------------------------
void ATopDownPlayerController::OnGameResumed_Implementation()
{
    Super::OnGameResumed_Implementation();
    //TODO@g: Hide pause menu
    SetMouseCursorWidget(EMouseCursor::Crosshairs, nullptr);
    BindInputMapping(MovementInputMapping);
    BindInputMapping(LookInputMapping);
    BindInputMapping(InteractionsInputMapping);
}
#pragma endregion Pause