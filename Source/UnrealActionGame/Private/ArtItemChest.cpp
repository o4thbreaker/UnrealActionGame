// Fill out your copyright notice in the Description page of Project Settings.


#include "ArtItemChest.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AArtItemChest::AArtItemChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	RootComponent = BaseMesh;

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>("LidMesh");
	LidMesh->SetupAttachment(BaseMesh);

	TargetPitch = 120;

	// now UE will keep an eye on this script.
	// it will update the variables when they will be changed (only interesting ones ofc)
	SetReplicates(true);
}


void AArtItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	IsLidOpened = !IsLidOpened;

	// call manually to notify the server
	// otherwise, only client will update the lid
	OnRep_LidOpened();
}

// Called when the game starts or when spawned
void AArtItemChest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AArtItemChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AArtItemChest::OnRep_LidOpened()
{
	float CurrentPitch = IsLidOpened ? TargetPitch : 0.0f;
	LidMesh->SetRelativeRotation(FRotator(CurrentPitch, 0, 0));
}

void AArtItemChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifeTimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifeTimeProps);

	// whenever IsLidOpened is changed, send this to all clients
	DOREPLIFETIME(AArtItemChest, IsLidOpened);
}