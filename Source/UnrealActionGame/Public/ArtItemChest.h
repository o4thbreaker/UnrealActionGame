 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArtGameplayInterface.h"
#include "ArtItemChest.generated.h"

class UStaticMeshComponent;

UCLASS()
class UNREALACTIONGAME_API AArtItemChest : public AActor, public IArtGameplayInterface
{
	GENERATED_BODY()

public:

	void Interact_Implementation(APawn* InstigatorPawn);

	void OnActorLoaded_Implementation();

	UPROPERTY(EditAnywhere)
	float TargetPitch;
	
public:	
	// Sets default values for this actor's properties
	AArtItemChest();

protected:

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UStaticMeshComponent* LidMesh;

	UPROPERTY(ReplicatedUsing="OnRep_LidOpened", BlueprintReadOnly, SaveGame) // RepNotify
	bool IsLidOpened;

	// always bind to UFUNCTION things that UE uses behind the scenes (e.g. ReplicatedUsing)
	UFUNCTION()
	void OnRep_LidOpened();
};
