// Fill out your copyright notice in the Description page of Project Settings.


#include "ArtAnimInstance.h"
#include "GameplayTagContainer.h"
#include "ArtActionComponent.h"

void UArtAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	AActor* OwnerActor = GetOwningActor();
	if (OwnerActor)
	{
		ActionComponent = Cast<UArtActionComponent>(OwnerActor->GetComponentByClass(UArtActionComponent::StaticClass()));
	}
}

void UArtAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// does player have stunned tag
	// since this is a part of core gameplay we can boldly assume that it exists
	static FGameplayTag StunnedTag = FGameplayTag::RequestGameplayTag("Status.Stunned");

	if (ActionComponent)
	{
		isStunned = ActionComponent->ActiveGameplayTags.HasTag(StunnedTag);
	}
}
