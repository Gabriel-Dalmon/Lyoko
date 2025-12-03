// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Items/ItemBase.h"

AItemBase::AItemBase()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);
}

void AItemBase::OnConstruction(const FTransform &Transform)
{
	Super::OnConstruction(Transform);

	// Apply the mesh in both editor and runtime
	if (Mesh)
	{
		MeshComponent->SetStaticMesh(Mesh);
	}
	else
	{
		MeshComponent->SetStaticMesh(nullptr);
	}
}

#if WITH_EDITOR
/**
 * Called when a property is changed in the editor
 * @param PropertyChangedEvent - Event that was triggered
*/
void AItemBase::PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName PropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if (PropertyName == GET_MEMBER_NAME_CHECKED(AItemBase, Mesh))
	{
		if (Mesh)
		{
			MeshComponent->SetStaticMesh(Mesh);
		}
		else
		{
			MeshComponent->SetStaticMesh(nullptr);
		}
	}
}
#endif
