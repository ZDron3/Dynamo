// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "Walls.generated.h"

class APowerUps;
/**
 *
 */
UCLASS(Blueprintable, BlueprintType)
class DYNAMO_API AWalls : public AActor
{
	GENERATED_BODY()

public:
	bool  IsBreakable() const { return bIsBreakable; }
	float GetDropChance() const { return DropChance; }

	const TArray<TSubclassOf<APowerUps>>& GetDropList() const { return DropList; }

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
	UPROPERTY(EditAnywhere)
		TArray<TSubclassOf<APowerUps>> DropList;

	UPROPERTY(EditAnywhere)
		float DropChance = 0.3f;

	UPROPERTY(EditAnywhere)
		bool bIsBreakable = false;

	virtual void BeginPlay() override;

private:
	void SpawnPowerup();
};
