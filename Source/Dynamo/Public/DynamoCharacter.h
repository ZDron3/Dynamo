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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	//	UGridMovementComponent* GridMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 MaxBombs = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 BombRange = 3;

	// Active remote controlled bomb effects count.
	// When it's down to 0, the player will not drop remote controlled bombs.
	// When it's equal to one or more, the player will drop remote controlled bombs.
	UPROPERTY(BlueprintReadWrite)
		int32 RemoteControlPowerupEffectCount = 0;

	int32 CurrentBombCount = 0;

	UFUNCTION(BlueprintPure, Category = "BombermanClone")
		int32 GetScore() const;
	void  IncrementScore();

	/*UGridMovementComponent* GetGridMovementComponent() const { return GridMovementComponent; }*/

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
	UFUNCTION(BlueprintPure, Category = "Bomberman", meta = (DisplayName = "GetPlayerControllerId"))
		void K2_GetPlayerControllerId(bool& OutIsPlayerHuman, int32& OutPlayerControllerId) const;

	UFUNCTION(BlueprintImplementableEvent, Category = "Player2Init")
		void SetMeshRotation(float SetMeshForward, float SetMeshLeft);

private:
	void MoveX(const float AxisInput);
	void MoveY(const float AxisInput);
	void MoveForward_Backward(float Val);
	void MoveLeft_Right(float Val);
	

	void DetonateRemoteControlledBombs();

	void DropBomb();


};

