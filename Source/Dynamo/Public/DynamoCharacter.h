// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DynamoCharacter.generated.h"

class ABombs;
class UGridMovementComponent;

UCLASS()
class DYNAMO_API ADynamoCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ADynamoCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 MaxBombs = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 BombRange = 2;

  //bool to check whether the remote power is acquired if yes then initialize the bomb data accordingly
	UPROPERTY(BlueprintReadWrite)
	bool RemoteControlPowerupEffect = false;

	int32 CurrentBombCount = 0;

	UFUNCTION(BlueprintPure, Category = "Dynamo")
		int32 GetScore() const;
	void  IncrementScore();


	// Returns the Controller ID if the Local Player is available, otherwise returns an empty object.
	TOptional<int32> GetPlayerControllerId() const;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	static TArray<ADynamoCharacter*> GetAllPlayerPawns(UWorld* const World);

protected:
	UPROPERTY(EditAnywhere)
		TSubclassOf<ABombs> BombClass;

	UPROPERTY(EditAnywhere)
		int32 BombLifetimeSeconds = 2.f;

	// Returns the Controller ID if a human player controls this pawn, otherwise returns -1.
	UFUNCTION(BlueprintPure, Category = "Dynamo", meta = (DisplayName = "GetPlayerControllerId"))
		void Dynamo_GetPlayerControllerId(bool& OutIsPlayerHuman, int32& OutPlayerControllerId) const;


private:
	void MoveForward_Backward(float Val);
	void MoveLeft_Right(float Val);

	void DetonateRemoteControlledBombs();

	void DropBomb();


};

