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

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
