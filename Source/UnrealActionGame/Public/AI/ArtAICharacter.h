// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ArtAICharacter.generated.h"

class UPawnSensingComponent;
class UArtAttributeComponent;

UCLASS()
class UNREALACTIONGAME_API AArtAICharacter : public ACharacter
{
	GENERATED_BODY()

public:

	AArtAICharacter();

protected:

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TimeToHitParamName;

	void SetTargetActor(AActor* NewTarget);

	virtual void PostInitializeComponents() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UArtAttributeComponent* AttributeComponent;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UArtAttributeComponent* OwningComp, float NewHealth, float Delta);

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);
};
