// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bombs.generated.h"

class ALevelManager;
class ADynamoCharacter;
class AParticleActor;

UCLASS()
class DYNAMO_API ABombs : public AActor
{
	GENERATED_BODY()
	


public:
	ABombs();

	void InitializeBombData(ADynamoCharacter* const InOwnerPawn,
		const float        InLifetimeSeconds,
		const int32        InExplosionRange,
		const bool         bInIsRemoteControlled);

	UFUNCTION(BlueprintPure, Category = "Dynamo")
		bool IsRemoteControlled() const { return bIsRemoteControlled; }

	ADynamoCharacter* GetOwnerPawn() const { return OwnerPawn; }

	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AParticleActor> ParticleClass;

	virtual void BeginPlay() override;

private:
	bool bIsRemoteControlled = false;

	int32 ExplosionRange = 1;

	UPROPERTY()
		ADynamoCharacter* OwnerPawn;

	// Cached level grid pointer
	UPROPERTY()
	ALevelManager* LevelManager;

	// Triggers an explosion at the current location.
	void Explode();

	// Recursive explosion function. Will destroy one cell and continue in the indicated direction until the range runs
	// out, or an unbreakable block is hit.
	void ExplosionStep(const FIntPoint& Location, const FIntPoint& Direction, const int32 Range);
};
