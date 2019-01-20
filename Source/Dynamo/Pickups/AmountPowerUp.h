// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePowerUp.h"
#include "AmountPowerUp.generated.h"

/**
 * 
 */
UCLASS()
class DYNAMO_API AAmountPowerUp : public ABasePowerUp
{
	GENERATED_BODY()
	
public:
	/** Default constructor
	*/
	AAmountPowerUp();
	
protected:
	/** Called when this pick up is picked by player.
	*/
	virtual void PickedUpBy_Implementation(ADynamoCharacter* Character) override;

private:

	/** Number of extra bombs that will be available for player.
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Pickup|CarpedBombPickup", meta = (MinValue = 0))
	int32 IncrementAmount;
};
