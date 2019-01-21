// Fill out your copyright notice in the Description page of Project Settings.

#include "DynamoCharacter.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"	
#include "EngineUtils.h"
#include "DynamoGameInstance.h"
#include "Bombs.h"
#include "Kismet/GameplayStatics.h"
#include "DynamoGameModeBase.h"


// Sets default values
ADynamoCharacter::ADynamoCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

int32 ADynamoCharacter::GetScore() const
{
	// Retrieve the score from the GameInstance.

	UWorld* const World = GetWorld();
	check(IsValid(World));

	UDynamoGameInstance* const GameInstance = World->GetGameInstanceChecked<UDynamoGameInstance>();

	const TOptional<int32> PlayerControllerId = GetPlayerControllerId();

	if (!PlayerControllerId.IsSet())
	{
		return 0;
	}
	return GameInstance->GetPlayerScore(PlayerControllerId.GetValue());
}

void ADynamoCharacter::IncrementScore()
{
	// Store the score from the GameInstance so it presists between matches.

	UWorld* const World = GetWorld();
	check(IsValid(World));

	const TOptional<int32> PlayerControllerId = GetPlayerControllerId();
	if (!PlayerControllerId.IsSet())
	{
		return;
	}

	UDynamoGameInstance* const GameInstance = World->GetGameInstanceChecked<UDynamoGameInstance>();
	GameInstance->IncrementPlayerScore(PlayerControllerId.GetValue());
}

void ADynamoCharacter::SetupPlayerInputComponent(UInputComponent* const PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	const TOptional<int32> ControllerId = GetPlayerControllerId();

	if (!ControllerId.IsSet())
	{
		return;
	}

	check(IsValid(PlayerInputComponent));

	// Part of the hack for local player keyboard sharing descibed in

	if (ControllerId == 0)
	{ //temp todo
		//InputComponent->BindAxis(TEXT("Player0_MoveX"), this, &ThisClass::MoveX);
		//InputComponent->BindAxis(TEXT("Player0_MoveY"), this, &ThisClass::MoveY);
		InputComponent->BindAxis("MoveForward_Backward", this, &ADynamoCharacter::MoveForward_Backward);
		InputComponent->BindAxis("MoveLeft_Right", this, &ADynamoCharacter::MoveLeft_Right);
		InputComponent->BindAction(TEXT("Player0_DropBomb"), IE_Pressed, this, &ADynamoCharacter::DropBomb);
		InputComponent->BindAction(TEXT("Player0_DetonateBombs"), IE_Pressed, this, &ADynamoCharacter::DetonateRemoteControlledBombs);
	}
	else if (ControllerId == 1)
	{
		//temp todo
		/*InputComponent->BindAxis(TEXT("Player1_MoveX"), this, &ThisClass::MoveX);
		InputComponent->BindAxis(TEXT("Player1_MoveY"), this, &ThisClass::MoveY);*/
		InputComponent->BindAxis("MoveForward_Backward2", this, &ADynamoCharacter::MoveForward_Backward);
		InputComponent->BindAxis("MoveLeft_Right2", this, &ADynamoCharacter::MoveLeft_Right);
		
		InputComponent->BindAction(TEXT("Player1_DropBomb"), IE_Pressed, this, &ADynamoCharacter::DropBomb);
		InputComponent->BindAction(TEXT("Player1_DetonateBombs"), IE_Pressed, this, &ADynamoCharacter::DetonateRemoteControlledBombs);
	}
	else
	{
		checkNoEntry();
	}
}
void ADynamoCharacter::MoveForward_Backward(float Val)
{
	if (Val != 0.0f)
	{
		// find out which way is forward
		
		const FVector Direction = FRotationMatrix(GetControlRotation()).GetScaledAxis(EAxis::X);
		// Limit pitch when walking or falling
		AddMovementInput(Direction, Val);
		SetMeshRotation(Val, 0);
	}
}
void ADynamoCharacter::MoveLeft_Right(float Val)
{
	if (Val != 0.0f)
	{
		// find out which way is Right

		const FVector Direction = FRotationMatrix(GetControlRotation()).GetScaledAxis(EAxis::Y);
		// Limit pitch when walking or falling
		AddMovementInput(Direction, Val);
		SetMeshRotation(Val, 0);
	}
}


	void ADynamoCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
	{
		Super::EndPlay(EndPlayReason);

		UWorld* const World = GetWorld();
		check(IsValid(World));

		ADynamoGameModeBase* GameMode = CastChecked<ADynamoGameModeBase>(World->GetAuthGameMode());
	    GameMode->NotifyPlayerDied();
	}

	TArray<ADynamoCharacter*> ADynamoCharacter::GetAllPlayerPawns(UWorld* const World)
	{
		TArray<ADynamoCharacter*> AllPlayerPawns;
		for (ADynamoCharacter* const PlayerPawn : TActorRange<ADynamoCharacter>{ World })
		{
			if (IsValid(PlayerPawn))
			{
				AllPlayerPawns.Add(PlayerPawn);
			}
		}
		return AllPlayerPawns;
	}

	void ADynamoCharacter::K2_GetPlayerControllerId(bool& OutIsPlayerHuman, int32& OutPlayerControllerId) const
	{
		const TOptional<int32> ControllerId = GetPlayerControllerId();

		if (!ControllerId.IsSet())
		{
			OutIsPlayerHuman = false;
			OutPlayerControllerId = -1;
		}
		else
		{
			OutIsPlayerHuman = true;
			OutPlayerControllerId = ControllerId.GetValue();
		}
	}

	void ADynamoCharacter::BeginPlay()
	{
		Super::BeginPlay();
	//	ALevelManager::SnapActorToGrid(this); //get using gamemode
	}

	TOptional<int32> ADynamoCharacter::GetPlayerControllerId() const
	{
		const APlayerController* const PlayerController = Cast<APlayerController>(GetController());
		if (!IsValid(PlayerController))
		{
			return TOptional<int32>{};
		}
		const ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
		if (!IsValid(LocalPlayer))
		{
			return TOptional<int32>{};
		}
		return LocalPlayer->GetControllerId();
	}

	void ADynamoCharacter::MoveX(const float AxisInput)
	{
		//check(IsValid(GridMovementComponent));
		// GridMovementComponent->Move(-FVector::RightVector, AxisInput); not needed
	}

	void ADynamoCharacter::MoveY(const float AxisInput)
	{
		//check(IsValid(GridMovementComponent));
		//	GridMovementComponent->Move(FVector::ForwardVector, AxisInput); not needed
	}

	void ADynamoCharacter::DetonateRemoteControlledBombs()
	{
		RemoteControlPowerupEffectCount = 0;
		for (ABombs* const Bomb : TActorRange<ABombs>{ GetWorld() })
		{
			
			if (IsValid(Bomb) && Bomb->GetOwnerPawn() == this && Bomb->IsRemoteControlled())
			{
				Bomb->Destroy();
			}
		}
	}

	void ADynamoCharacter::DropBomb()
	{
		if (!IsValid(BombClass))
		{
			UE_LOG(LogTemp, Error, TEXT("BombClass not set."));
			return;
		}

		check(CurrentBombCount >= 0);

		if (CurrentBombCount >= MaxBombs)
		{
		//	 Bomb limit reached, spawn no more.
			return;
		}

		UWorld* const World = GetWorld();
		check(IsValid(World));

		const FTransform SpawnTransform{ GetActorLocation() };

		ABombs* const Bomb = World->SpawnActorDeferred<ABombs>(BombClass, SpawnTransform);
		check(IsValid(Bomb));

		Bomb->InitializeBombData(this, BombLifetimeSeconds, BombRange, RemoteControlPowerupEffectCount > 0);
		UGameplayStatics::FinishSpawningActor(Bomb, SpawnTransform);
	}

