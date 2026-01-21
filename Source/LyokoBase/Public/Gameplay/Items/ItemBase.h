// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemBase.generated.h"

UCLASS(Abstract)
class LYOKOBASE_API AItemBase : public AActor
{
	GENERATED_BODY()
	
protected:
	/* Mesh Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> MeshComponent;

public:
	AItemBase();
};
