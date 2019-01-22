// Fill out your copyright notice in the Description page of Project Settings.

#include "Bombs.h"
#include "Pickups/BasePowerUp.h"
#include "DynamoCharacter.h"
#include "LevelManager.h"
#include "Engine/World.h"
#include "ParticleActor.h"
#include "Walls.h"

// Sets default values
ABombs::ABombs()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ABombs::InitializeBombData(ADynamoCharacter* const InOwnerPawn, const float InLifetimeSeconds, const int32 InExplosionRange, const bool bInIsRemoteControlled)
{
	OwnerPawn = InOwnerPawn;
	ExplosionRange = InExplosionRange;
	bIsRemoteControlled = bInIsRemoteControlled;

	if (!bInIsRemoteControlled)
	{
		InitialLifeSpan = InLifetimeSeconds;
	}
}

void ABombs::BeginPlay()
{
	Super::BeginPlay();

	// Increment owner Bombs count
	if (IsValid(OwnerPawn))
	{
		OwnerPawn->CurrentBombCount++;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s has no owner pawn."), *GetName());
	}

	ALevelManager::ActorToGrid(this);

	LevelManager = ALevelManager::Get(GetWorld());
}
//todo
void ABombs::Explode()
{
	UWorld* const World = GetWorld();
	check(IsValid(World));

	check(IsValid(LevelManager));
	const FIntPoint Epicenter = LevelManager->GetActorCell(this);

	const auto StartExplosionBranch = [&](const FIntPoint& Direction) {
		ExplosionStep(Epicenter + Direction, Direction, ExplosionRange);
	};

	StartExplosionBranch(FIntPoint{ 1, 0 });  // Right
	StartExplosionBranch(FIntPoint{ 0, -1 }); // Down
	StartExplosionBranch(FIntPoint{ -1, 0 }); // Left
	StartExplosionBranch(FIntPoint{ 0, 1 });  // Up

	ExplosionStep(Epicenter, FIntPoint{ 0, 0 }, 1); // Center
}

void ABombs::ExplosionStep(const FIntPoint& Location, const FIntPoint& Direction, const int32 Range)
{
	if (Range == 0)
	{
		// Stop condition: explosion force ran out.
		return;
	}

	for (ABasePowerUp* const PowerUp : LevelManager->GetActorsInCell<ABasePowerUp>(Location))
	{
		check(IsValid(PowerUp));
		PowerUp->Destroy();
	}


	check(IsValid(LevelManager));
	//todo
	for (AWalls* const Wall : LevelManager->GetActorsInCell<AWalls>(Location))
	{
		check(IsValid(Wall));
		if (Wall->IsBreakable())
		{
			Wall->Destroy();
			//change to macro todo
			if (FMath::RandRange(1, 100 / 25) == 1 ? true : false)
			{
				LevelManager->SpawnPowerUps(Wall, FMath::RandRange(1, 4));
			}
			return;

		}
		else
		{
			// Stop condition: hit an unbreakable block.
			return;
		}
	}

	for (ADynamoCharacter* const PlayerPawn : LevelManager->GetActorsInCell<ADynamoCharacter>(Location))
	{
		check(IsValid(PlayerPawn));
		PlayerPawn->Destroy();
	}

	for (ABombs* const Bombs : LevelManager->GetActorsInCell<ABombs>(Location))
	{
		check(IsValid(Bombs));
		Bombs->Destroy();
	}


	// Spawn explosion block
	UWorld* const World = GetWorld();
	check(IsValid(World));

	if (!IsValid(ParticleClass))
	{
		UE_LOG(LogTemp, Error, TEXT("%s: Particle class not set."), *GetName());
		return;
	}

	const FVector ExplosionWorldLocation = LevelManager->GetWorldLocationFromCell(Location);

	AParticleActor* const Particle =	World->SpawnActor<AParticleActor>(ParticleClass, ExplosionWorldLocation, FRotator::ZeroRotator);
	check(IsValid(Particle));
	ExplosionStep(Location + Direction, Direction, Range - 1);
}

void ABombs::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (EndPlayReason == EEndPlayReason::Destroyed)
	{
		// Decrement owner Bombs count
		if (IsValid(OwnerPawn))
		{
			OwnerPawn->CurrentBombCount--;
			check(OwnerPawn->CurrentBombCount >= 0);
		}

		Explode();
	}
}