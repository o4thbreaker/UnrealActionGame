// Fill out your copyright notice in the Description page of Project Settings.
#include "ArtTargetDummy.h"
#include "ArtAttributeComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AArtTargetDummy::AArtTargetDummy()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	RootComponent = MeshComponent;

	AttributeComponent = CreateDefaultSubobject<UArtAttributeComponent>("AttributeComponent");
	AttributeComponent->OnHealthChanged.AddDynamic(this, &AArtTargetDummy::OnHealthChanged);
}

void AArtTargetDummy::OnHealthChanged(AActor* InstigatorActor, UArtAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.0f)
	{
		MeshComponent->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
	}
}

