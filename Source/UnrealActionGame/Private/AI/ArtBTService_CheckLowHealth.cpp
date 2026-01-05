// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ArtBTService_CheckLowHealth.h"
#include "ArtAttributeComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void UArtBTService_CheckLowHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UArtAttributeComponent* AttributeComp = Cast<UArtAttributeComponent>(OwnerComp.GetAIOwner()->GetPawn()->GetComponentByClass(UArtAttributeComponent::StaticClass()));

	if (ensure(AttributeComp))
	{
		UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
		if (AttributeComp->GetHealth() <= HealthTreshold)
		{
			BlackboardComponent->SetValueAsBool(IsLowHealthRangeKey.SelectedKeyName, true);
		}
		else
		{
			BlackboardComponent->SetValueAsBool(IsLowHealthRangeKey.SelectedKeyName, false);
		}
	}
}
