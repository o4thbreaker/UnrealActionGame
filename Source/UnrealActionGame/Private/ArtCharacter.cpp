// Fill out your copyright notice in the Description page of Project Settings.


#include "ArtCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "ArtInteractionComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Kismet/KismetMathLibrary.h>
#include "DrawDebugHelpers.h"
#include "ArtAttributeComponent.h"

// Sets default values
AArtCharacter::AArtCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SetupAttachment(RootComponent);
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);

	InteractionComponent = CreateDefaultSubobject<UArtInteractionComponent>("InteractionComponent");

	AttributeComponent = CreateDefaultSubobject<UArtAttributeComponent>("AttributeComponent");

	GetCharacterMovement()->bOrientRotationToMovement = true;

	bUseControllerRotationYaw = false; 

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
	PlayerInputComponent->BindAxis("MoveRight", this, &AArtCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &AArtCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("BlackholeAttack", IE_Pressed, this, &AArtCharacter::BlackholeAttack);
	PlayerInputComponent->BindAction("TeleportAttack", IE_Pressed, this, &AArtCharacter::TeleportAttack);

	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &AArtCharacter::PrimaryInteract);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

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

	// x - forward (red)
	// y - right (green)
	// z - up (blue)

	// gets right vector of camera regardless of pawn's rotation i think
	FVector RightVector = FRotationMatrix(ControlRotation).GetScaledAxis(EAxis::Y);

	AddMovementInput(RightVector, Value);
}

void AArtCharacter::PrimaryAttack()
{
	PlayAnimMontage(AttackAnimation);

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &AArtCharacter::PrimaryAttack_TimeElapsed, 0.2f);
}

void AArtCharacter::PrimaryAttack_TimeElapsed()
{
	// always triggers the assert if class is null
	if (ensureAlways(ProjectileClass))
	{
		FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = this;

		// params for line trace
		FHitResult HitResult;
		FCollisionObjectQueryParams ObjectQueryParams;
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);

		//line trace itself. TODO: fix the magic number
		FVector CameraEndVector = CameraComponent->GetComponentLocation() + (CameraComponent->GetComponentRotation().Vector() * 5000.0f);
		bool bTraceResult = GetWorld()->LineTraceSingleByObjectType(HitResult, CameraComponent->GetComponentLocation(), CameraEndVector, ObjectQueryParams);
		DrawDebugLine(GetWorld(), HandLocation, CameraEndVector, FColor::Cyan, false, 1.0f, 0U, 2.0f);
		
		FVector TargetRotationEndVector = bTraceResult ? HitResult.ImpactPoint : CameraEndVector;
		FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(HandLocation, TargetRotationEndVector);

		FTransform SpawnTransform = FTransform(TargetRotation, HandLocation);
		
		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTransform, SpawnParams);
	}
}

void AArtCharacter::BlackholeAttack()
{
	PlayAnimMontage(AttackAnimation);

	GetWorldTimerManager().SetTimer(TimerHandle_BlackholeAttack, this, &AArtCharacter::BlackholeAttack_TimeElapsed, 0.2f);
}

// need to refactor to one function for spawning projectiles (move them to another class?)
void AArtCharacter::BlackholeAttack_TimeElapsed()
{
	if (ensureAlways(BlackholeProjectileClass))
	{
		FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = this;

		// params for line trace
		FHitResult HitResult;
		FCollisionObjectQueryParams ObjectQueryParams;
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);

		//line trace itself. TODO: fix the magic number
		FVector CameraEndVector = CameraComponent->GetComponentLocation() + (CameraComponent->GetComponentRotation().Vector() * 5000.0f);
		bool bTraceResult = GetWorld()->LineTraceSingleByObjectType(HitResult, CameraComponent->GetComponentLocation(), CameraEndVector, ObjectQueryParams);
		DrawDebugLine(GetWorld(), HandLocation, CameraEndVector, FColor::Cyan, false, 1.0f, 0U, 2.0f);

		FVector TargetRotationEndVector = bTraceResult ? HitResult.ImpactPoint : CameraEndVector;
		FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(HandLocation, TargetRotationEndVector);

		FTransform SpawnTransform = FTransform(TargetRotation, HandLocation);

		GetWorld()->SpawnActor<AActor>(BlackholeProjectileClass, SpawnTransform, SpawnParams);
	}
}

void AArtCharacter::TeleportAttack()
{
	PlayAnimMontage(AttackAnimation);

	GetWorldTimerManager().SetTimer(TimerHandle_TeleportAttack, this, &AArtCharacter::TeleportAttack_TimeElapsed, 0.2f);
}

void AArtCharacter::TeleportAttack_TimeElapsed()
{
	if (ensureAlways(TeleportProjectileClass))
	{
		FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = this;

		// params for line trace
		FHitResult HitResult;
		FCollisionObjectQueryParams ObjectQueryParams;
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);

		//line trace itself. TODO: fix the magic number
		FVector CameraEndVector = CameraComponent->GetComponentLocation() + (CameraComponent->GetComponentRotation().Vector() * 5000.0f);
		bool bTraceResult = GetWorld()->LineTraceSingleByObjectType(HitResult, CameraComponent->GetComponentLocation(), CameraEndVector, ObjectQueryParams);
		DrawDebugLine(GetWorld(), HandLocation, CameraEndVector, FColor::Cyan, false, 1.0f, 0U, 2.0f);

		FVector TargetRotationEndVector = bTraceResult ? HitResult.ImpactPoint : CameraEndVector;
		FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(HandLocation, TargetRotationEndVector);

		FTransform SpawnTransform = FTransform(TargetRotation, HandLocation);

		GetWorld()->SpawnActor<AActor>(TeleportProjectileClass, SpawnTransform, SpawnParams);
	}
}

void AArtCharacter::PrimaryInteract()
{
	InteractionComponent->PrimaryInteract();
}
