// Copyright © 2025-2027 Lyoko - 96 l'Art Cheperdu
//     __                __       
//    / /   __  ______  / /______ 
//   / /   / / / / __ \/ //_/ __ \
//  / /___/ /_/ / /_/ / ,< / /_/ /
// /_____/\__, /\____/_/|_|\____/ 
//       /____/                   

#pragma once

//----------------------------------------------------------------------------------------------------------------------
#include "CoreMinimal.h"
#include "Model/LyokoProperty.h"
#include "Items/Properties/ReloadPatterns.h"
#include "ReloadProperty.generated.h"

//----------------------------------------------------------------------------------------------------------------------
/**
 * 
 */
UCLASS()
class LYOKOWEAPONS_API UReloadProperty : public ULyokoProperty
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    int Capacity = 10;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    EReloadPattern ReloadPattern = EReloadPattern::Complete;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float ReloadDuration = 1.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "ReloadPattern == EReloadPattern::Incremental", EditConditionHides))
    int ReloadBatchSize = 1;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "ReloadPattern == EReloadPattern::Incremental", EditConditionHides))
    float ReloadBatchCooldown = 0.0f;
};
