// Fill out your copyright notice in the Description page of Project Settings.


#include "ArtGameModeBase.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "AI/ArtAICharacter.h"
#include "ArtAttributeComponent.h"
#include "EngineUtils.h"
#include "ArtCharacter.h"
#include "ArtPlayerState.h"
#include "Kismet/GameplayStatics.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("art.SpawnBots"), true, TEXT("Enable spawning bots via timer"), ECVF_Cheat);
static TAutoConsoleVariable<bool> CVarSpawnItems(TEXT("art.SpawnItems"), true, TEXT("Enable spawning pick up items via timer"), ECVF_Cheat);

/// \TODO: add EQS to spawn random items

AArtGameModeBase::AArtGameModeBase()
{
	SpawnBotsTimerInterval = 2.0f;
	SpawnPickupItemsTimerInterval = 60.0f;
}

void AArtGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &AArtGameModeBase::SpawnBotTimerElapsed, SpawnBotsTimerInterval, true);
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnPickUpItems, this, &AArtGameModeBase::SpawnPickUpItemsTimerElapsed, SpawnPickupItemsTimerInterval, true);
}

void AArtGameModeBase::KillAll()
{
	for (TActorIterator<AArtAICharacter> It(GetWorld()); It; ++It)
	{
		AArtAICharacter* Bot = *It;

		UArtAttributeComponent* AttributeComponent = UArtAttributeComponent::GetAttributes(Bot);
		if (ensure(AttributeComponent) && AttributeComponent->IsAlive())
		{
			/// \TODO: can pass in player for kill credit
			AttributeComponent->Kill(this);
		}
	}
}

void AArtGameModeBase::SpawnBotTimerElapsed()
{
	int32 NumberOfAliveBots = 0;

	if (!CVarSpawnBots.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Warning, TEXT("Bot spawning disabled via cvar 'CVarSpawnBots'"));
		return;
	}

	// TActorIterator is just a better (performance-wise) version of GetAllActorsOfClass
	for (TActorIterator<AArtAICharacter> It(GetWorld()); It; ++It)
	{
		AArtAICharacter* Bot = *It;

		UArtAttributeComponent* AttributeComp = UArtAttributeComponent::GetAttributes(Bot);
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
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &AArtGameModeBase::OnSpawnBotQueryCompleted);

	}
}

void AArtGameModeBase::SpawnPickUpItemsTimerElapsed()
{
	if (!CVarSpawnItems.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Warning, TEXT("Items spawning disabled via cvar 'CVarSpawnItems'"));
		return;
	}

	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnPickupItemsQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
	if (ensure(QueryInstance))
	{
		/// \TODO: add pathexist in eqs bp and preferable make querier as a player start (not as game mode as now)
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &AArtGameModeBase::OnSpawnItemsQueryCompleted);
	}
}

void AArtGameModeBase::OnSpawnBotQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
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

void AArtGameModeBase::OnSpawnItemsQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn items EQS Query failed"));
		return;
	}

	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

	if (Locations.Num() > 0)
	{
		GetWorld()->SpawnActor<AActor>(PickUpItemClass, Locations[0], FRotator::ZeroRotator);
	}
}

void AArtGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	if (ensure(Controller))
	{
		Controller->UnPossess();

		RestartPlayer(Controller);
	}
}

void AArtGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
	AArtCharacter* Player = Cast<AArtCharacter>(VictimActor);
	AArtAICharacter* Enemy = Cast<AArtAICharacter>(VictimActor);

	if (Player)
	{
		// local because mp mode will be added
		FTimerHandle TimerHandle_RespawnDelay;

		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "RespawnPlayerElapsed", Player->GetController());

		float RespawnDelay = 2.0f;
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate, 2.0f, false);
	}

	if (Enemy)
	{
		// gain 3 coins
		AArtPlayerState* PlayerState = Cast<AArtPlayerState>(Cast<AArtCharacter>(Killer)->GetPlayerState());

		if (PlayerState)
		{
			/// \TODO: transfer to tweakable variable
			int32 RewardAmount = 3;
			PlayerState->SetCoinsAmount(PlayerState->GetCoinsAmount() + RewardAmount);

			PlayerState->OnCoinsValueChanged.Broadcast(PlayerState->GetCoinsAmount());
		}
	}

	UE_LOG(LogTemp, Log, TEXT("OnActorKilled: Victim %s, Killer: %s"), *GetNameSafe(VictimActor), *GetNameSafe(Killer));
}
