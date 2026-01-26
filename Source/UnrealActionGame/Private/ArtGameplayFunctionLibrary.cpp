// Fill out your copyright notice in the Description page of Project Settings.


#include "ArtGameplayFunctionLibrary.h"
#include "ArtAttributeComponent.h"

bool UArtGameplayFunctionLibrary::ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount)
{
	UArtAttributeComponent* AttributeComponent = UArtAttributeComponent::GetAttributes(TargetActor);

	if (AttributeComponent)
	{
		return AttributeComponent->ApplyHealthChange(DamageCauser, -DamageAmount);
	}

	return false;
}

bool UArtGameplayFunctionLibrary::ApplyDirectionalDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount, const FHitResult& HitResult)
{
	if (ApplyDamage(DamageCauser, TargetActor, DamageAmount))
	{
		UPrimitiveComponent* HitComponent = HitResult.GetComponent();
		if (HitComponent && HitComponent->IsSimulatingPhysics(HitResult.BoneName))
		{
			// TIP: when working with direction always start with target and substract origin
			// Direction = Target - Origin
			FVector Direction = HitResult.TraceEnd - HitResult.TraceStart;
			Direction.Normalize();

			float impulseMultiplier = 300000.0f;
			HitComponent->AddImpulseAtLocation(Direction * impulseMultiplier, HitResult.ImpactPoint,HitResult.BoneName);
		}
		return true;
	}

	return false;
}
