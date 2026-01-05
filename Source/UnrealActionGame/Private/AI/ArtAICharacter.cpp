// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ArtAICharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "ArtAttributeComponent.h"

AArtAICharacter::AArtAICharacter()
{
    PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComponent");
    AttributeComponent = CreateDefaultSubobject<UArtAttributeComponent>("AttributeComponent");

    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AArtAICharacter::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    PawnSensingComponent->OnSeePawn.AddDynamic(this, &AArtAICharacter::OnPawnSeen);
    AttributeComponent->OnHealthChanged.AddDynamic(this, &AArtAICharacter::OnHealthChanged);
}

void AArtAICharacter::OnHealthChanged(AActor* InstigatorActor, UArtAttributeComponent* OwningComp, float NewHealth, float Delta)
{
    FString CombinedString = FString::Printf(TEXT("%f"), AttributeComponent->GetHealth());
    DrawDebugString(GetWorld(), GetActorLocation(), CombinedString, nullptr, FColor::Red, 1.0f, true);
}

void AArtAICharacter::OnPawnSeen(APawn* Pawn)
{
    AAIController* AIController = Cast<AAIController>(GetController());

    if (AIController)
    {
        UBlackboardComponent* BBComponent = AIController->GetBlackboardComponent();

        BBComponent->SetValueAsObject("TargetActor", Pawn);

        DrawDebugString(GetWorld(), GetActorLocation(), "SEE U", nullptr, FColor::White, 0.1f, false);
    }
}

