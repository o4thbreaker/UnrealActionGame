// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ArtAICharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "ArtAttributeComponent.h"
#include "BrainComponent.h"

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
    if (Delta < 0.0f)
    {
        FString CombinedString = FString::Printf(TEXT("Ouch!"), AttributeComponent->GetHealth());
        //DrawDebugString(GetWorld(), GetActorLocation(), CombinedString, nullptr, FColor::Green, 2.0f, true);

        if (NewHealth <= 0.0f)
        {
            // stop BT
            AAIController* AIC = Cast<AAIController>(GetController());
            if (AIC)
            {
                AIC->GetBrainComponent()->StopLogic("Killed");
            }
            // ragdoll
            GetMesh()->SetCollisionProfileName("Ragdoll");
            GetMesh()->SetAllBodiesSimulatePhysics(true);
            

            // delete actor after n seconds
            SetLifeSpan(10.0f);
        }
    }
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

