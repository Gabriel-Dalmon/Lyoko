// Copyright � 2026 Lyoko - 96 l'Art Cheperdu

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "LyokoAIControllerBase.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class LYOKOAI_API ALyokoAIControllerBase : public AAIController
{
    GENERATED_BODY()

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|Perception")
    UAIPerceptionComponent *PerceptionComponentBis;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI|Perception")
    UAISenseConfig_Sight *SightConfig;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI|Perception")
    UAISenseConfig_Hearing *HearingConfig;

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LyokoAI")
    TObjectPtr<class UBehaviorTree> BehaviorTreeAsset;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "AI|Blackboard")
    TObjectPtr<UBlackboardComponent> BlackboardComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LyokoAI|Blackboard Keys")
    FName BehaviorStateKey = TEXT("State");

public:
    ALyokoAIControllerBase();

    virtual void OnPossess(APawn *InPawn) override;

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "LyokoAI|Controller")
    void OnBehaviourStateChanged(FName ChangedKeyName);
    virtual void OnBehaviourStateChanged_Implementation(FName ChangedKeyName);

    EBlackboardNotificationResult InternalOnBehaviourStateChanged(const UBlackboardComponent &BlackboardComp, FBlackboard::FKey ChangedKeyID);


public:
    UFUNCTION(BlueprintCallable, Category = "AI|Perception")
    void SetHearingRadius(float Radius);
};
