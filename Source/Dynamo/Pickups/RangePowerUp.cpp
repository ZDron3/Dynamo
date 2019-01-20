// Fill out your copyright notice in the Description page of Project Settings.

#include "RangePowerUP.h"
#include "DynamoCharacter.h"
//#include "Characters/Weapon/BombPlacerComponent.h" //component for bombplacer not needed todo

ARangePowerUp::ARangePowerUp() :
	IncrementAmount(2.0f)
{

}

void ARangePowerUp::PickedUpBy_Implementation(ADynamoCharacter* Character)
{
	Character->BombRange++;
	Super::PickedUpBy_Implementation(Character);
}