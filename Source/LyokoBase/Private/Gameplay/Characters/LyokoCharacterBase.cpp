// Copyright � 2026 Lyoko - 96 l'Art Cheperdu

#include "Gameplay/Characters/LyokoCharacterBase.h"
#include "Gameplay/Characters/HealthComponent.h"

ALyokoCharacterBase::ALyokoCharacterBase()
{
    HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
}
