// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArtGameplayInterface.h"
#include "ArtBasePickUpItem.generated.h"

class UArtAttributeComponent;

UCLASS()
class UNREALACTIONGAME_API AArtBasePickUpItem : public AActor, public IArtGameplayInterface
{
	GENERATED_BODY()
	
public:	
	AArtBasePickUpItem();

protected:
	//virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	virtual void Interact_Implementation(APawn* InstitgatorPawn) override;

protected:

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UArtAttributeComponent* AttributeComponent;
};
