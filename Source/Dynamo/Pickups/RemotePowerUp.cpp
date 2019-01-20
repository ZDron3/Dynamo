// Fill out your copyright notice in the Description page of Project Settings.

#include "RemotePowerUp.h"
#include "DynamoCharacter.h"
//#include "Characters/Weapon/RemoteBombPlacerComponent.h" //component not needed use accordingly todo

ARemotePowerUp::ARemotePowerUp() :
	MaxPowerupTime(10.0f)
{

}

void ARemotePowerUp::PickedUpBy_Implementation(ADynamoCharacter* Character)
{

	Character->RemoteControlPowerupEffectCount += 1;
	Super::PickedUpBy_Implementation(Character);
}