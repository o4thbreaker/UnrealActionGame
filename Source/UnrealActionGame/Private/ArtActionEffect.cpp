// Fill out your copyright notice in the Description page of Project Settings.


#include "ArtActionEffect.h"
#include "ArtActionComponent.h"

UArtActionEffect::UArtActionEffect()
{
	IsAutoStart = true;
}

void UArtActionEffect::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	if (Duration > 0.0f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "StopAction", Instigator);

		GetWorld()->GetTimerManager().SetTimer(DurationHandle, Delegate, Duration, false);
	}
	if (Period > 0.0f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "ExecutePeriodicEffect", Instigator);

		GetWorld()->GetTimerManager().SetTimer(PeriodHandle, Delegate, Period, true);
	}
}

void UArtActionEffect::StopAction_Implementation(AActor* Instigator)
{
	// if duration is over buuuut we fairly have to do one more effect tick 
	if (GetWorld()->GetTimerManager().GetTimerRemaining(PeriodHandle) < KINDA_SMALL_NUMBER)
	{
		ExecutePeriodicEffect(Instigator);
	}

	Super::StopAction_Implementation(Instigator);

	GetWorld()->GetTimerManager().ClearTimer(PeriodHandle);
	GetWorld()->GetTimerManager().ClearTimer(DurationHandle);

	UArtActionComponent* Component = GetOwningComponent();

	if (Component)
	{
		Component->RemoveAction(this);
	}

}

float UArtActionEffect::GetTimeRemaining() const
{
	float EndTime = TimeStarted + Duration;

	return EndTime - GetWorld()->TimeSeconds;
}

void UArtActionEffect::ExecutePeriodicEffect_Implementation(AActor* Instigator)
{

}
