// Fill out your copyright notice in the Description page of Project Settings.


#include "ArtAction_ProjectileAttack.h"
#include "GameFramework/Character.h" 
#include "Kismet/GameplayStatics.h"

UArtAction_ProjectileAttack::UArtAction_ProjectileAttack()
{
	HandSocketName = "Muzzle_01";
	AttackAnimDelay = 0.2f;
}

void UArtAction_ProjectileAttack::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	ACharacter* Character = Cast<ACharacter>(Instigator);

	if (Character)
	{
		Character->PlayAnimMontage(AttackAnimation);

		UGameplayStatics::SpawnEmitterAttached(CastingEffect, Character->GetMesh(), HandSocketName,
			FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);


		if (Character->HasAuthority())
		{
			FTimerHandle TimerHandle_AttackDelay;
			FTimerDelegate Delegate;
			Delegate.BindUFunction(this, "AttackDelay_Elapsed", Character);

			GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay, Delegate, AttackAnimDelay, false);
		}
	}
}

void UArtAction_ProjectileAttack::AttackDelay_Elapsed(ACharacter* InstigatorCharacter)
{
	if (ensureAlways(ProjectileClass))
	{
		FVector HandLocation = InstigatorCharacter->GetMesh()->GetSocketLocation(HandSocketName);

		//params for trace
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = InstigatorCharacter;

		FCollisionShape Shape;
		Shape.SetSphere(20.0f);

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(InstigatorCharacter);

		FCollisionObjectQueryParams ObjectQueryParams;
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);

		/// \TODO: fix the magic number. or not
		FHitResult HitResult;
		FVector CameraStartVector = InstigatorCharacter->GetPawnViewLocation();
		FVector CameraEndVector = CameraStartVector + (InstigatorCharacter->GetControlRotation().Vector() * 5000.0f);

		bool bTraceResult = GetWorld()->SweepSingleByObjectType(HitResult, CameraStartVector, CameraEndVector,
			FQuat::Identity, ObjectQueryParams, Shape, QueryParams);

		DrawDebugLine(GetWorld(), HandLocation, CameraEndVector, FColor::Cyan, false, 1.0f, 0U, 2.0f);

		FVector TargetRotationEndVector = bTraceResult ? HitResult.ImpactPoint : CameraEndVector;

		// have no idea what that is ive googled it
		FRotator TargetRotation = FRotationMatrix::MakeFromX(CameraEndVector - HandLocation).Rotator();

		FTransform SpawnTransform = FTransform(TargetRotation, HandLocation);
		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTransform, SpawnParams);
	}

	StopAction(InstigatorCharacter);
}
