// Fill out your copyright notice in the Description page of Project Settings.

#include "Walls.h"
#include "LevelManager.h"
#include "PowerUps.h"

void AWalls::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (EndPlayReason == EEndPlayReason::Destroyed && FMath::FRand() < DropChance && DropList.Num() > 0)
	{
		SpawnPowerup();
	}
}

void AWalls::BeginPlay()
{
	Super::BeginPlay();
	ALevelManager::SnapActorToGrid(this);
}

void AWalls::SpawnPowerup()
{
	check(DropList.Num() > 0);

	const TSubclassOf<APowerUps> PowerupClass = DropList[FMath::RandRange(0, DropList.Num() - 1)];
	check(IsValid(PowerupClass));

	UWorld* const World = GetWorld();
	check(IsValid(World));

	APowerUps* const Powerup = World->SpawnActor<APowerUps>(PowerupClass, GetActorLocation(), FRotator::ZeroRotator);
	check(IsValid(Powerup));
}
