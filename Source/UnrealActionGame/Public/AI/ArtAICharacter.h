// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ArtAICharacter.generated.h"

class UPawnSensingComponent;
class UArtAttributeComponent;
class UUserWidget;
class UArtWorldUserWidget;
class UArtActionComponent;

UCLASS()
class UNREALACTIONGAME_API AArtAICharacter : public ACharacter
{
	GENERATED_BODY()

public:

	AArtAICharacter();

protected:

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TimeToHitParamName;

	UArtWorldUserWidget* ActiveHealthBar;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;

	void SetTargetActor(AActor* NewTarget);

	virtual void PostInitializeComponents() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UArtAttributeComponent* AttributeComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UArtActionComponent* ActionComponent;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UArtAttributeComponent* OwningComp, float NewHealth, float Delta);

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);
};
