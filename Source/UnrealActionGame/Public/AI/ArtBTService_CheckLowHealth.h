// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "ArtBTService_CheckLowHealth.generated.h"

/**
 * 
 */
UCLASS()
class UNREALACTIONGAME_API UArtBTService_CheckLowHealth : public UBTService
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "AI")
	float HealthTreshold = 30.0f;

	UPROPERTY(EditAnywhere, Category = "AI");
	FBlackboardKeySelector IsLowHealthRangeKey;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
