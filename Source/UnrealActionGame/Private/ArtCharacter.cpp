// Fill out your copyright notice in the Description page of Project Settings.


#include "ArtCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
AArtCharacter::AArtCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(RootComponent);
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);

}

// Called when the game starts or when spawned
void AArtCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AArtCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AArtCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AArtCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Turn", this, &AArtCharacter::AddControllerYawInput);

}

void AArtCharacter::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector(), Value);
}

