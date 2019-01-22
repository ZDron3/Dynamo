// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EngineUtils.h"
#include "GameFramework/Actor.h"
#include "LevelManager.generated.h"

class AWalls;
class ABasePowerUp;
class AAmountPowerUp;
class ARangePowerUp;
class ASprintPowerUp;
class ARemotePowerUp;


UCLASS()
class DYNAMO_API ALevelManager : public AActor
{
	GENERATED_BODY()

public:
	ALevelManager();

	// Returns the LevelGrid actor for this level.
	static ALevelManager* Get(UWorld* const World);

	// Location to the center of its cell in the grid.
	FVector CenterToGrid(const FVector& WorldLocation) const;

	static void ActorToGrid(AActor* const Actor);

	// Returns all actors of the given type located in a grid cell.
	//todo
	template <typename ActorType>
	TArray<ActorType*> GetActorsInCell(const FIntPoint& GridCoords) const
	{
		TArray<ActorType*> Actors;
		for (ActorType* const Actor : TActorRange<ActorType>{ GetWorld() })
		{
			check(::IsValid(Actor));
			if (this->GetActorCell(Actor) == GridCoords)
			{
				Actors.Add(Actor);
			}
		}
		return Actors;
	}
	
	void SpawnPowerUps(AWalls* location,int Choice) const;

	// Gets the cell coordinates for an actor.
	FIntPoint GetActorCell(const AActor* const Actor) const;

	// Converts grid coordinates to world coordinates.
	FVector GetWorldLocationFromCell(const FIntPoint& GridCoords) const;

	// Converts world coordinates to grid coordinates.
	FIntPoint GetCellFromWorldLocation(const FVector& WorldLocation) const;

	virtual void OnConstruction(const FTransform& Transform) override;

	TArray<FIntPoint> FreeCords;

protected:
	// Number of cells in each dimension in the grid.
	//changes can be made here to change the cell amount and size
	UPROPERTY(EditAnywhere)
		FIntPoint GridSize = FIntPoint{ 13, 13 };

	// Size of a single cell
	UPROPERTY(EditAnywhere)
		float CellSize = 150.f;
	//walls

	UPROPERTY(EditAnywhere)
		TSubclassOf<AWalls> WallClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<AWalls> UnbreakableBlockClass;

	UPROPERTY(EditDefaultsOnly, Category = "PowerUps")
		TSubclassOf<AWalls> BreakableBlockClass;

	//powerups

	UPROPERTY(EditDefaultsOnly, Category = "PowerUps")
		TSubclassOf<AAmountPowerUp> AmountPowerUpClass;

	UPROPERTY(EditDefaultsOnly, Category = "PowerUps")
		TSubclassOf<ARangePowerUp> RangePowerUpClass;

	UPROPERTY(EditDefaultsOnly, Category = "PowerUps")
		TSubclassOf<ASprintPowerUp> SprintPowerUpClass;

	UPROPERTY(EditDefaultsOnly, Category = "PowerUps")
		TSubclassOf<ARemotePowerUp> RemotePowerUpClass;

	bool Decide = false;


private:
	// Spawns the blocks that are present in every level.
	void SpawnBaseBlocks();
	
	// Creates a block in a given cell.
	void CreateBlock(const TSubclassOf<AWalls> WallClass, const FIntPoint& GridCoords);

	void ClearAllGeneratedBlocks();
};
