// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ArtAIController.generated.h"

/**
 * 
 */
UCLASS()
class UNREALACTIONGAME_API AArtAIController : public AAIController
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBehaviorTree* BehaviourTree;
	virtual void BeginPlay() override;

};
