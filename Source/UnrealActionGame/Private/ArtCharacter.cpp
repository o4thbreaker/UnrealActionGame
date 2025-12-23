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

	HandSocketName = "Muzzle_01";

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

	// gets right vector of camera regardless of pawn's rotation i think
	FVector RightVector = FRotationMatrix(ControlRotation).GetScaledAxis(EAxis::Y);

	AddMovementInput(RightVector, Value);
}

void AArtCharacter::PrimaryAttack()
{
	PlayAnimMontage(AttackAnimation);

	///\TODO: doesn't work for some reason (maybe because i haven't attached casting effect in constructor?)

	/*UGameplayStatics::SpawnEmitterAttached(CastingEffectComponent, GetMesh(), HandSocketName, 
		FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);*/

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &AArtCharacter::PrimaryAttack_TimeElapsed, 0.2f);
}

void AArtCharacter::PrimaryAttack_TimeElapsed()
{
	SpawnProjectile(ProjectileClass);
}

void AArtCharacter::BlackholeAttack()
{
	PlayAnimMontage(AttackAnimation);

	GetWorldTimerManager().SetTimer(TimerHandle_BlackholeAttack, this, &AArtCharacter::BlackholeAttack_TimeElapsed, 0.2f);
}

void AArtCharacter::BlackholeAttack_TimeElapsed()
{
	SpawnProjectile(BlackholeProjectileClass);
}

void AArtCharacter::TeleportAttack()
{
	PlayAnimMontage(AttackAnimation);

	GetWorldTimerManager().SetTimer(TimerHandle_TeleportAttack, this, &AArtCharacter::TeleportAttack_TimeElapsed, 0.2f);
}

void AArtCharacter::TeleportAttack_TimeElapsed()
{
	SpawnProjectile(TeleportProjectileClass);
}

void AArtCharacter::SpawnProjectile(TSubclassOf<AActor> ClassToSpawn)
{
	if (ensureAlways(ClassToSpawn))
	{
		FVector HandLocation = GetMesh()->GetSocketLocation(HandSocketName);

		//params for trace
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = this;

		FCollisionShape Shape;
		Shape.SetSphere(20.0f);

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);

		FCollisionObjectQueryParams ObjectQueryParams;
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);

		//line trace itself. TODO: fix the magic number
		FHitResult HitResult;
		FVector CameraStartVector = CameraComponent->GetComponentLocation();
		FVector CameraEndVector = CameraComponent->GetComponentLocation() + (CameraComponent->GetComponentRotation().Vector() * 5000.0f);

		bool bTraceResult = GetWorld()->SweepSingleByObjectType(HitResult, CameraStartVector, CameraEndVector,
			FQuat::Identity, ObjectQueryParams, Shape, QueryParams);

		DrawDebugLine(GetWorld(), HandLocation, CameraEndVector, FColor::Cyan, false, 1.0f, 0U, 2.0f);

		FVector TargetRotationEndVector = bTraceResult ? HitResult.ImpactPoint : CameraEndVector;

		// have no idea what that is i googled it
		FRotator TargetRotation = FRotationMatrix::MakeFromX(CameraEndVector - HandLocation).Rotator();

		FTransform SpawnTransform = FTransform(TargetRotation, HandLocation);

		GetWorld()->SpawnActor<AActor>(ClassToSpawn, SpawnTransform, SpawnParams);
	}
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
	}
}
