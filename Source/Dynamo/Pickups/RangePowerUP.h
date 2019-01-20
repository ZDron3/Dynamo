// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePowerUp.h"
#include "RangePowerUp.generated.h"

/**
 * 
 */
UCLASS()
class DYNAMO_API ARangePowerUp : public ABasePowerUp
{
	GENERATED_BODY()
	
public:
	/** Default constructor
	*/
	ARangePowerUp();
protected:

	/** Called when this pick up is picked by player.
	*/
	virtual void PickedUpBy_Implementation(ADynamoCharacter* Character) override;
	
private:
	/** Number of extra tiles to add to explosion
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Pickup|MegaBombPickup", meta = (MinValue = 0))
	int32 IncrementAmount;
};
