// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePowerUp.h"
#include "SprintPowerUp.generated.h"

/**
 * 
 */
UCLASS()
class DYNAMO_API ASprintPowerUp : public ABasePowerUp
{
	GENERATED_BODY()
	
public:
	/** Default constructor
	*/
	ASprintPowerUp();

protected:
	/** Called when this pick up is picked by player.
	*/
	virtual void PickedUpBy_Implementation(ADynamoCharacter* Character) override;

private:
	/** Factor at which 
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Pickup|SprintPickup")
	float SpeedMultiplier;
};
