// Fill out your copyright notice in the Description page of Project Settings.


#include "ArtGameModeBase.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "AI/ArtAICharacter.h"
#include "ArtAttributeComponent.h"
#include "EngineUtils.h"


AArtGameModeBase::AArtGameModeBase()
{
	SpawnTimerInterval = 2.0f;
}

void AArtGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &AArtGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);
}

void AArtGameModeBase::SpawnBotTimerElapsed()
{
	int32 NumberOfAliveBots = 0;

	// TActorIterator is just a better (performance-wise) version of GetAllActorsOfClass
	for (TActorIterator<AArtAICharacter> It(GetWorld()); It; ++It)
	{
		AArtAICharacter* Bot = *It;

		// bloody mess
		/// \TODO: add attribute component to ai to resolve bug
		UArtAttributeComponent* AttributeComp = Cast<UArtAttributeComponent>(Bot->GetComponentByClass(UArtAttributeComponent::StaticClass()));
		if (ensure(AttributeComp) && AttributeComp->IsAlive())
		{
			NumberOfAliveBots++;
		}
	}

	float MaxBotCount = 10.0f;

	if (DifficultyCurve)
	{
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}

	if (NumberOfAliveBots >= MaxBotCount)
	{
		UE_LOG(LogTemp, Log, TEXT("Maximum capacity of bots spawning"));
		return;
	}

	// dont know why we're passubg this as querier and not the players
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
	if (ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &AArtGameModeBase::OnQueryCompleted);

	}
}

void AArtGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query failed"));
		return;
	}
	
	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

	if (Locations.Num() > 0)
	{
		GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);
	}
}
