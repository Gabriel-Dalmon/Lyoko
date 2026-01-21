// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Items/ItemBase.h"

AItemBase::AItemBase()
{
	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent")));

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);
}