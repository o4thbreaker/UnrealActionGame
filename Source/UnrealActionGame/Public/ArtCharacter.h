// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ArtCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UArtInteractionComponent;
class UAnimMontage;

UCLASS()
class UNREALACTIONGAME_API AArtCharacter : public ACharacter
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass;
	
	UPROPERTY(EditAnywhere, Category="Attack")
	UAnimMontage* AttackAnimation;

	FTimerHandle TimerHandle_PrimaryAttack;

public:
	// Sets default values for this character's properties
	AArtCharacter();

protected:
	
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere)
	UArtInteractionComponent* InteractionComponent;


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);

	void MoveRight(float Value);

	void PrimaryAttack();

	void PrimaryAttack_TimeElapsed();

	void PrimaryInteract();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
