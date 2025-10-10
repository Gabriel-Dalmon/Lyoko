// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/LyokoGameModeBase.h"
#include "Gameplay/Characters/LyokoCharacterBase.h"
#include "Controllers/LyokoPlayerControllerBase.h"
#include "Core/HooksInterfaces/OnPlayerRestartedHook.h"


void ALyokoGameModeBase::Respawn(ALyokoPlayerControllerBase *Controller)
{
	FTransform RespawnPoint = Controller->RespawnPoint;
	if (RespawnPoint.IsValid() == false)
	{
		RespawnPoint = FTransform(FRotator(0, 0, 0), FVector(0, 0, 0));
	}

	UWorld *World = GetWorld();
	ALyokoCharacterBase *Character =
		World->SpawnActor<ALyokoCharacterBase>(DefaultPawnClass, RespawnPoint.GetLocation(), RespawnPoint.GetRotation().Rotator());
	Controller->Possess(Character);
}

void ALyokoGameModeBase::FinishRestartPlayer(AController *NewPlayer, const FRotator &StartRotation)
{
    Super::FinishRestartPlayer(NewPlayer, StartRotation);

    if (!NewPlayer) return;

    if (NewPlayer->GetClass()->ImplementsInterface(UOnPlayerRestartedHook::StaticClass()))
    {
		IOnPlayerRestartedHook::Execute_OnPlayerRestarted(NewPlayer);
    }
}
