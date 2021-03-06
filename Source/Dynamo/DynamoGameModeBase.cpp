// Fill out your copyright notice in the Description page of Project Settings.

#include "DynamoGameModeBase.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "DynamoCharacter.h"
#include "LevelManager.h"
#include "Engine/World.h"


static constexpr float SecondsInOneMinute = 60.f;

void ADynamoGameModeBase::NotifyPlayerDied()
{
	// We can't pick the winner yet. The second player might die from the same blast. Let's check again in the next
	// tick.
	GetWorldTimerManager().SetTimerForNextTick(this, &ADynamoGameModeBase::OnPostPlayerDiedTick);
}

void ADynamoGameModeBase::StartPlay()
{
	Super::StartPlay();

	const UWorld* const World = GetWorld();
	check(IsValid(World));
	StartTime = World->GetTimeSeconds();

	// When the time runs out, invoke EndMatch with nullptr. It's a draw.
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUObject(this, &ADynamoGameModeBase::EndMatch, static_cast<ADynamoCharacter*>(nullptr));
	GetWorldTimerManager().SetTimer(MatchTimerHandle, TimerDelegate, TimeLimitMinutes * SecondsInOneMinute, false);
	const TArray<ADynamoCharacter*> PlayerCharacter = ADynamoCharacter::GetAllPlayerPawns(GetWorld());
	LevelManager = GetWorld()->SpawnActor<ALevelManager>(ALevelManager::StaticClass());
	for (ADynamoCharacter* Character : PlayerCharacter)
	{
		Character->SetActorLocation(LevelManager->GetWorldLocationFromCell(LevelManager->FreeCords[FMath::RandRange(1,(LevelManager->FreeCords.Num()-1))]));
	}

}

void ADynamoGameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorldTimerManager().ClearAllTimersForObject(this);

}

float ADynamoGameModeBase::GetSecondsLeftInMatch() const
{
	const UWorld* const World = GetWorld();
	check(IsValid(World));
	const float EndTime = StartTime + TimeLimitMinutes * SecondsInOneMinute;
	return EndTime - World->GetTimeSeconds();
}

void ADynamoGameModeBase::OnPostPlayerDiedTick()
{
	if (bIsMatchOver)
	{
		// Match is already over.
		return;
	}

	const TArray<ADynamoCharacter*> AllPlayerPawns = ADynamoCharacter::GetAllPlayerPawns(GetWorld());


	if (AllPlayerPawns.Num() == 2)
	{
		// All players are alive. Continue the match.
	}
	else if (AllPlayerPawns.Num() == 1)
	{
		// We have a winner.
		EndMatch(AllPlayerPawns[0]);
	}
	else if (AllPlayerPawns.Num() == 0)
	{
		// It's a draw.
		EndMatch(nullptr);
	}
}

void ADynamoGameModeBase::EndMatch(ADynamoCharacter* const Winner)
{
	bIsMatchOver = true;
	if (IsValid(Winner))
	{
		Winner->IncrementScore();
	}

	UGameplayStatics::SetGamePaused(this, /*bPaused*/ true);
	OnMatchEnded(Winner);
}