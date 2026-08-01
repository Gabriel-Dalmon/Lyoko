// Copyright � 2026 Lyoko - 96 l'Art Cheperdu
//     __                __       
//    / /   __  ______  / /______ 
//   / /   / / / / __ \/ //_/ __ \
//  / /___/ /_/ / /_/ / ,< / /_/ /
// /_____/\__, /\____/_/|_|\____/ 
//       /____/                   

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LyokoCharacterBase.generated.h"

UCLASS(Abstract)
class LYOKOBASE_API ALyokoCharacterBase : public ACharacter
{
    GENERATED_BODY()

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
    TObjectPtr<class UHealthComponent > HealthComponent;

public:
    ALyokoCharacterBase();
};
