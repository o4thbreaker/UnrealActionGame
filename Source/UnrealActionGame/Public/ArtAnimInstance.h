// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ArtAnimInstance.generated.h"

class UArtActionComponent;

/**
 * 
 */
UCLASS()
class UNREALACTIONGAME_API UArtAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	UArtActionComponent* ActionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	bool isStunned;

	/* BeginPlay of AnimInstance */
	void NativeInitializeAnimation() override;

	/* Tick of AnimInstance */
	void NativeUpdateAnimation(float DeltaSeconds) override;
};
