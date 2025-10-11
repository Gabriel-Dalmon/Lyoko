// Copyright © 2025 Lyoko - 96 l'Art Cheperdu


#include "Gameplay/Characters/LyokoCharacterBase.h"
#include "Gameplay/Characters/HealthComponent.h"

// Sets default values
ALyokoCharacterBase::ALyokoCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
}

// Called when the game starts or when spawned
void ALyokoCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALyokoCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ALyokoCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

