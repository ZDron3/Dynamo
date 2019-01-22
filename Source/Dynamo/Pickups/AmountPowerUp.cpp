// Fill out your copyright notice in the Description page of Project Settings.

#include "AmountPowerUp.h"
#include "DynamoCharacter.h"

AAmountPowerUp::AAmountPowerUp() :
	IncrementAmount(2.0f)
{

}

void AAmountPowerUp::PickedUpBy_Implementation(ADynamoCharacter* Character)
{

	Character->MaxBombs++;
	Super::PickedUpBy_Implementation(Character);
}
