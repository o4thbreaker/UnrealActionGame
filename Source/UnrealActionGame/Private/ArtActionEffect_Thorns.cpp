// Fill out your copyright notice in the Description page of Project Settings.


#include "ArtActionEffect_Thorns.h"
#include "ArtActionComponent.h"
#include "ArtAttributeComponent.h"

UArtActionEffect_Thorns::UArtActionEffect_Thorns()
{
	Duration = 0.0f;
	DamagePercentage = 10.0f;
}

void UArtActionEffect_Thorns::StartAction_Implementation(AActor* Instigator)
{
	UArtAttributeComponent* AttributeComponent = UArtAttributeComponent::GetAttributes(Instigator);

	AttributeComponent->OnHealthChanged.AddDynamic(this, &UArtActionEffect_Thorns::DamageBack);
}

void UArtActionEffect_Thorns::StopAction_Implementation(AActor* Instigator)
{
	UArtAttributeComponent* AttributeComponent = UArtAttributeComponent::GetAttributes(Instigator);

	AttributeComponent->OnHealthChanged.RemoveDynamic(this, &UArtActionEffect_Thorns::DamageBack);
}

void UArtActionEffect_Thorns::DamageBack(AActor* DamageCauser, UArtAttributeComponent* OwningComponent, float NewHealth, float Delta)
{
	AActor* Owner = GetOwningComponent()->GetOwner();

	float Coeffecient = FMath::Abs((Delta * DamagePercentage) / 100.0f);

	float Damage = FMath::CeilToInt(Coeffecient);

	/*FString DamageMsg = FString::Printf(TEXT("Delta: %.2f. Damage: %.2f. Delta/100: %.2f"), Delta, Damage, Coeffecient);
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, DamageMsg);*/

	if (Delta < 0.0f && DamageCauser != Owner)
	{
		// apply health change to the instigator
		UArtAttributeComponent* DamageCauserAttributeComponent = UArtAttributeComponent::GetAttributes(DamageCauser);
		DamageCauserAttributeComponent->ApplyHealthChange(Owner, Damage);
	}
}
