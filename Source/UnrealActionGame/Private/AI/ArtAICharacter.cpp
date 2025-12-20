// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ArtAICharacter.h"

// Sets default values
AArtAICharacter::AArtAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AArtAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AArtAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


