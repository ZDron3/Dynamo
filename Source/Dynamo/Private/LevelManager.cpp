// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelManager.h"
#include "Walls.h"
#include "Pickups/BasePowerUp.h"
#include "Pickups/AmountPowerUp.h"
#include "Pickups/RangePowerUp.h"
#include "Pickups/RemotePowerUp.h"
#include "Pickups/SprintPowerUp.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "EngineUtils.h"


ALevelManager::ALevelManager()
{
	PrimaryActorTick.bCanEverTick = false;
	//todo setup init components using bP for levelmanager
	static ConstructorHelpers::FClassFinder<AActor>WallTemplateClass(TEXT("/Game/BP/InDestructable"));
	if (WallTemplateClass.Class != NULL)
	{
		WallClass = WallTemplateClass.Class;
	}
	static ConstructorHelpers::FClassFinder<AActor>UnbreakableBlockTemplateClass(TEXT("/Game/BP/InDestructable"));
	if (UnbreakableBlockTemplateClass.Class != NULL)
	{
		UnbreakableBlockClass = UnbreakableBlockTemplateClass.Class;
	}
	static ConstructorHelpers::FClassFinder<AActor>BreakableBlockTemplateClass(TEXT("/Game/BP/Destructable"));
	if (BreakableBlockTemplateClass.Class != NULL)
	{
		BreakableBlockClass = BreakableBlockTemplateClass.Class;
	}

	static ConstructorHelpers::FClassFinder<AActor> AmountPowerUpClassTemplateClass(TEXT("/Game/BP/PowerUp/BP_AmountPowerUp"));
	if (AmountPowerUpClassTemplateClass.Class != NULL)
	{
		AmountPowerUpClass = AmountPowerUpClassTemplateClass.Class;
	}

	static ConstructorHelpers::FClassFinder<AActor> RangePowerUpTemplateClass(TEXT("/Game/BP/PowerUp/BP_RangePowerUp"));
	if (RangePowerUpTemplateClass.Class != NULL)
	{
		RangePowerUpClass = RangePowerUpTemplateClass.Class;
	}

	static ConstructorHelpers::FClassFinder<AActor> RemotePowerUpTemplateClass(TEXT("/Game/BP/PowerUp/BP_RemotePowerUp"));
	if (RemotePowerUpTemplateClass.Class != NULL)
	{
		RemotePowerUpClass = RemotePowerUpTemplateClass.Class;
	}

	static ConstructorHelpers::FClassFinder<AActor> SprintPowerUpTemplateClass(TEXT("/Game/BP/PowerUp/BP_SprintPowerUp"));
	if (SprintPowerUpTemplateClass.Class != NULL)
	{
		SprintPowerUpClass = SprintPowerUpTemplateClass.Class;
	}
}

void ALevelManager::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	ClearAllGeneratedBlocks();
	SpawnBaseBlocks();
}

ALevelManager* ALevelManager::Get(UWorld* const World)
{
	ALevelManager* FoundLevel = nullptr;
	for (ALevelManager* const Level : TActorRange<ALevelManager>{ World })

	{
		if (!IsValid(Level))
		{
			continue;
		}
		if (!FoundLevel)
		{
			FoundLevel = Level;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("More than one ALevelManager exists in the level."));
		}
	}
	return FoundLevel;
}
void ALevelManager::SpawnBaseBlocks()
{
	// Iterate through every grid cell
	for (int32 GridX = 0; GridX < GridSize.X; ++GridX)
	{
		for (int32 GridY = 0; GridY < GridSize.Y; ++GridY)
		{
			const FIntPoint GridCoords{ GridX, GridY };

			// Spawn wall blocks at the the borders of the level
			if (GridCoords.X == 0 || GridCoords.X == GridSize.X - 1 || GridCoords.Y == 0 ||
				GridCoords.Y == GridSize.Y - 1)
			{
				if (IsValid(WallClass))
				{
					CreateBlock(WallClass, GridCoords);
				}
			}
			// Place an unbreakable block every even cell
			else if (GridCoords.X % 2 == 0 && GridCoords.Y % 2 == 0)
			{
				if (IsValid(UnbreakableBlockClass))
				{
					CreateBlock(UnbreakableBlockClass, GridCoords);
				}
			}
			else
			{
				Decide = FMath::RandBool();
					if (Decide)
					{
						CreateBlock(BreakableBlockClass, GridCoords);
					}
					else
					{
						FreeCords.Add(GridCoords);
					}
				}
				
			}
		}
	}

//walls todo
void ALevelManager::CreateBlock(const TSubclassOf<AWalls> BlockClass,
	const FIntPoint&          GridCoords)
{
	// Make sure the coordinates are valid
	check(GridCoords.X >= 0 && GridCoords.X < GridSize.X);
	check(GridCoords.Y >= 0 && GridCoords.Y < GridSize.Y);

	const FVector CellWorldLocation = GetWorldLocationFromCell(GridCoords);

	UChildActorComponent* const ChildActorComponent = NewObject<UChildActorComponent>(this);
	check(IsValid(ChildActorComponent));
	ChildActorComponent->SetChildActorClass(BlockClass);
	ChildActorComponent->SetWorldLocation(CellWorldLocation);
	ChildActorComponent->RegisterComponent();
}

void ALevelManager::ClearAllGeneratedBlocks()
{
	TArray<UChildActorComponent*> ChildActorComponents;
	GetComponents<UChildActorComponent>(/*out*/ ChildActorComponents);

	for (UChildActorComponent* const ChildActorComponent : ChildActorComponents)
	{
		if (IsValid(ChildActorComponent))
		{
			ChildActorComponent->DestroyComponent();
		}
	}
}


FVector ALevelManager::GetWorldLocationFromCell(const FIntPoint& GridCoords) const
{
	// The grid is centered at world location (0, 0, CellSize/2)
	return FVector{ FVector2D{GridCoords - GridSize / 2}, 0.5f } *CellSize;
}

FIntPoint ALevelManager::GetCellFromWorldLocation(const FVector& WorldLocation) const
{
	return FIntPoint{ FMath::RoundToInt(WorldLocation.X / CellSize), FMath::RoundToInt(WorldLocation.Y / CellSize) } +
		GridSize / 2;
}

FIntPoint ALevelManager::GetActorCell(const AActor* const Actor) const
{
	check(IsValid(Actor));
	return GetCellFromWorldLocation(Actor->GetActorLocation());
}

FVector ALevelManager::SnapToGrid(const FVector& WorldLocation) const
{
	const FIntPoint Cell = GetCellFromWorldLocation(WorldLocation);
	return GetWorldLocationFromCell(Cell);
}

void ALevelManager::SnapActorToGrid(AActor* const Actor)
{
	check(IsValid(Actor));

	const ALevelManager* const LevelManager = Get(Actor->GetWorld());
	check(IsValid(LevelManager));

	const FVector SnappedPosition = LevelManager->SnapToGrid(Actor->GetActorLocation());
	Actor->SetActorLocation(SnappedPosition);
}

void ALevelManager::BeginPlay()
{
	Super::BeginPlay();
  //todo player spawn logic wrt to proc generation
}

void ALevelManager::SpawnPowerUps(AWalls * location, int Choice) const
{
	if (Choice != 0)
	{
		if (Choice == 1)
		{
			ABasePowerUp* pickup = GetWorld()->SpawnActor<ABasePowerUp>(AmountPowerUpClass, location->GetActorTransform());
		}
		else if (Choice == 2)
		{
			ABasePowerUp* pickup = GetWorld()->SpawnActor<ABasePowerUp>(RangePowerUpClass, location->GetActorTransform());
		}
		else if (Choice == 3)
		{
			ABasePowerUp* pickup = GetWorld()->SpawnActor<ABasePowerUp>(RemotePowerUpClass, location->GetActorTransform());
		}
		else if (Choice == 4)
		{
			ABasePowerUp* pickup = GetWorld()->SpawnActor<ABasePowerUp>(SprintPowerUpClass, location->GetActorTransform());
		}
	}
}

