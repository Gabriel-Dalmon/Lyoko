// Fill out your copyright notice in the Description page of Project Settings.


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
#include "GameFramework/CharacterMovementComponent.h"



void ATopDownPlayerController::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
}

void ATopDownPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem *Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(MovementMappingContext, 0);
	}

	// Set up actions bindings
	if (UEnhancedInputComponent *EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(MovementInputAction, ETriggerEvent::Triggered, this, &ATopDownPlayerController::Move);
		EnhancedInputComponent->BindAction(PauseInputAction, ETriggerEvent::Triggered, this, &ATopDownPlayerController::PauseGame);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ATopDownPlayerController::Move(const FInputActionValue &Value)
{
	APawn *ControlledPawn = GetPawn();
	if (ControlledPawn == nullptr) return;

	const FVector2d Axes = Value.Get<FVector2D>();

	const FRotator YawRotation(0, GetControlRotation().Yaw, 0);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	ControlledPawn->AddMovementInput(ForwardDirection, Axes.Y);
	ControlledPawn->AddMovementInput(RightDirection, Axes.X);
}

void ATopDownPlayerController::LookAtCursor()
{
	FHitResult OutHit;
	const bool bHit = GetHitResultUnderCursor(ECC_GameTraceChannel1, true, OutHit);
	if (bHit == false) return;

	APawn *ControlledPawn = GetPawn();
	if (ControlledPawn == nullptr) return;
	
	FRotator currentCharacterRotation = ControlledPawn->GetActorRotation();

	FRotator PlayerRot = UKismetMathLibrary::FindLookAtRotation(ControlledPawn->GetActorLocation(), OutHit.Location);

	FRotator newRotation = FRotator(currentCharacterRotation.Pitch, PlayerRot.Yaw, PlayerRot.Roll);
	ControlledPawn->SetActorRotation(newRotation);
}

void ATopDownPlayerController::OnPossess(APawn *InPawn)
{
	Super::OnPossess(InPawn);
	SetActorTickEnabled(true);
	RespawnPoint = InPawn->GetActorTransform();
	ACharacter* PossessedCharacter = Cast<ACharacter>(InPawn);

	// Get the component ref from the interface or cast directly and move to PlayerControllerBase
	if (UHealthComponent *HealthComponent = InPawn->FindComponentByClass<UHealthComponent>())
	{
		HealthComponent->OnDeadEvent.AddDynamic(this, &ATopDownPlayerController::OnPossessedPawnDead);
	}

	if (UCameraComponent *CameraComponent = InPawn->FindComponentByClass<UCameraComponent>())
	{
		FRotator CameraRotation = CameraComponent->GetComponentRotation();
		CameraRotation.Pitch = 0.0f;
		CameraRotation.Roll = 0.0f;
		SetControlRotation(CameraRotation);
	}
}

void ATopDownPlayerController::OnUnPossess()
{
	Super::OnUnPossess();

	SetActorTickEnabled(false);
}

void ATopDownPlayerController::OnPossessedPawnDead()
{
	ALyokoGameModeBase *GameMode = Cast<ALyokoGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode == nullptr) return;
	GameMode->Respawn(this);
}

void ATopDownPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	cooldown += DeltaSeconds;

	// TODO@g - figure out why ControlRotation gets reset after OnPossess and remove this
	if (cooldown < 1.0f)
	{
		APawn *ControlledPawn = GetPawn();
		if (ControlledPawn)
		{
			if (UCameraComponent *CameraComponent = ControlledPawn->FindComponentByClass<UCameraComponent>())
			{
				FRotator CameraRotation = CameraComponent->GetComponentRotation();
				CameraRotation.Pitch = 0.0f;
				CameraRotation.Roll = 0.0f;
				SetControlRotation(CameraRotation);
			}
		}
	}
	LookAtCursor();
}
