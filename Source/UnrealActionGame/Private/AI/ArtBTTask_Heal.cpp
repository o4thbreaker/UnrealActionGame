// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ArtBTTask_Heal.h"
#include "AIController.h"
#include "ArtAttributeComponent.h"

EBTNodeResult::Type UArtBTTask_Heal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* MyController = OwnerComp.GetAIOwner();

	if (ensure(MyController))
	{
		APawn* AIPawn = MyController->GetPawn();
		if (ensure(AIPawn))
		{
			UArtAttributeComponent* AttributeComp = Cast<UArtAttributeComponent>(AIPawn->GetComponentByClass(UArtAttributeComponent::StaticClass()));
			if (ensure(AttributeComp))
			{
				AttributeComp->ApplyHealthChange(MyController, AttributeComp->GetMaxHealth());
				return EBTNodeResult::Succeeded;
			}
		}
	}
	return EBTNodeResult::Failed;
}
