// Fill out your copyright notice in the Description page of Project Settings.


#include "ArtHealthPotion.h"

void AArtHealthPotion::Interact_Implementation(APawn* InstitgatorPawn)
{
	// 1. check if instigator has an attribute component (get component by class of pawn (dont hard connect to character))
	// 2. check if current health of instigator isnt maximum (ignore instigator otherwise)
	// 3. give health to the instigator
	// 4. make potion uninteractable for 10 seconds (make mesh invisible + uninteractable + turn off collisions)
}

