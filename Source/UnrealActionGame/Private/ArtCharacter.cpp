// Fill out your copyright notice in the Description page of Project Settings.


#include "ArtCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "ArtInteractionComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Kismet/KismetMathLibrary.h>
#include "DrawDebugHelpers.h"
#include "ArtAttributeComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include <Kismet/GameplayStatics.h>
#include "ArtActionComponent.h"

AArtCharacter::AArtCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SetupAttachment(RootComponent);
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);

	InteractionComponent = CreateDefaultSubobject<UArtInteractionComponent>("InteractionComponent");

	AttributeComponent = CreateDefaultSubobject<UArtAttributeComponent>("AttributeComponent");

	ActionComponent = CreateDefaultSubobject<UArtActionComponent>("ActionComponent");

	GetCharacterMovement()->bOrientRotationToMovement = true;

	bUseControllerRotationYaw = false;

	BlackholeRageCost = 10.0f;
}

void AArtCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AArtCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComponent->OnHealthChanged.AddDynamic(this, &AArtCharacter::OnHealthChanged);
}

FVector AArtCharacter::GetPawnViewLocation() const
{
	return CameraComponent->GetComponentLocation();
}

void AArtCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AArtCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AArtCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AArtCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &AArtCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("BlackholeAttack", IE_Pressed, this, &AArtCharacter::BlackholeAttack);
	PlayerInputComponent->BindAction("TeleportAttack", IE_Pressed, this, &AArtCharacter::TeleportAttack);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AArtCharacter::SprintStart);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AArtCharacter::SprintStop);

	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &AArtCharacter::PrimaryInteract);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
}

void AArtCharacter::HealSelf(float Amount /* = 100 */)
{
	AttributeComponent->ApplyHealthChange(this, Amount);
}

void AArtCharacter::MoveForward(float Value)
{
	FRotator ControlRotation = GetControlRotation();
	ControlRotation.Pitch = 0.0f;
	ControlRotation.Roll = 0.0f;

	AddMovementInput(ControlRotation.Vector(), Value);
}

void AArtCharacter::MoveRight(float Value)
{
	FRotator ControlRotation = GetControlRotation();
	ControlRotation.Pitch = 0.0f;
	ControlRotation.Roll = 0.0f;

	// gets right vector of camera regardless of pawn's rotation i think
	FVector RightVector = FRotationMatrix(ControlRotation).GetScaledAxis(EAxis::Y);

	AddMovementInput(RightVector, Value);
}

void AArtCharacter::SprintStart()
{
	ActionComponent->StartActionByName(this, "Sprint");
}

void AArtCharacter::SprintStop()
{
	ActionComponent->StopActionByName(this, "Sprint");
}

void AArtCharacter::PrimaryAttack()
{
	ActionComponent->StartActionByName(this, "PrimaryAttack");
}

void AArtCharacter::BlackholeAttack()
{
	/// \TODO: probably not the best design idea to put this logic and not in Blackhole_Action?
	if (AttributeComponent->GetRage() >= BlackholeRageCost)
	{
		ActionComponent->StartActionByName(this, "Blackhole");
		AttributeComponent->LoseRage(BlackholeRageCost);
	}
}

void AArtCharacter::TeleportAttack()
{
	ActionComponent->StartActionByName(this, "Teleport");
}

void AArtCharacter::PrimaryInteract()
{
	InteractionComponent->PrimaryInteract();
}

void AArtCharacter::OnHealthChanged(AActor* InstigatorActor, UArtAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.0f)
	{
		FString CombinedString = FString::Printf(TEXT("Got hit!"));
		DrawDebugString(GetWorld(), GetActorLocation(), CombinedString, nullptr, FColor::Green, 2.0f, true);

		GetMesh()->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
	}
	if (NewHealth <= 0.0f && Delta < 0.0f)
	{
		APlayerController* PController = Cast<APlayerController>(GetController());
		DisableInput(PController);

		SetLifeSpan(4.0f);
	}
}
