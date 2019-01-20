// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePowerUp.h"
#include "RemotePowerUp.generated.h"

/**
 * 
 */
UCLASS()
class DYNAMO_API ARemotePowerUp : public ABasePowerUp
{
	GENERATED_BODY()
public:
	/** Default constructor
	*/
	ARemotePowerUp();

protected:

	/** Called when this pick up is picked by player.
	*/
	virtual void PickedUpBy_Implementation(ADynamoCharacter* Character) override;
	
	/** Amount of time this pick up works
	*/
	UPROPERTY(EditDefaultsOnly, Category = "BombPlacer", meta = (MinValue = 0))
	float MaxPowerupTime;
};
