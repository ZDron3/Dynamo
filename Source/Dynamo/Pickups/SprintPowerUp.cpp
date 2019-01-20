// Fill out your copyright notice in the Description page of Project Settings.

#include "SprintPowerUp.h"
#include "DynamoCharacter.h"
// Engine includes
#include "GameFramework/CharacterMovementComponent.h"

ASprintPowerUp::ASprintPowerUp() :
	SpeedMultiplier(2.0f)
{
}

void ASprintPowerUp::PickedUpBy_Implementation(ADynamoCharacter* Character)
{
	Character->GetCharacterMovement()->MaxAcceleration += 100;
	Character->GetCharacterMovement()->MaxWalkSpeed += 100;
	Super::PickedUpBy_Implementation(Character);
}
