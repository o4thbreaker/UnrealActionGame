// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArtGameplayInterface.h"
#include "ArtBasePickUpItem.generated.h"

class UArtAttributeComponent;
class USphereComponent;

UCLASS()
class UNREALACTIONGAME_API AArtBasePickUpItem : public AActor, public IArtGameplayInterface
{
	GENERATED_BODY()
	
public:	
	AArtBasePickUpItem();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "Collision")
	USphereComponent* InteractCollisionComponent;

	FTimerHandle TimerHandle_SetVisibility;

protected:
	virtual void Interact_Implementation(APawn* InstitgatorPawn) override;

	UFUNCTION()
	void HideItem();

	UFUNCTION()
	void SetVisibility_TimeElapsed();
};
