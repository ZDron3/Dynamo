// Fill out your copyright notice in the Description page of Project Settings.

#include "DynamoGameInstance.h"
#include "DynamoCharacter.h"

void UDynamoGameInstance::IncrementPlayerScore(const int32 PlayerId)
{
	PlayerScores.FindOrAdd(PlayerId);
	PlayerScores[PlayerId]++;

	CachedScoreString = GenerateScoreString();
}

int32 UDynamoGameInstance::GetPlayerScore(const int32 PlayerId) const
{
	if (const int32* const SearchResult = PlayerScores.Find(PlayerId))
	{
		return *SearchResult;
	}
	else
	{
		return 0;
	}
}

FString UDynamoGameInstance::GetScoreString() const
{
	if (CachedScoreString.IsEmpty())
	{
		CachedScoreString = GenerateScoreString();
	}
	return CachedScoreString;
}

FString UDynamoGameInstance::GenerateScoreString() const
{
	TArray<FString> ScoreLines;

	for (int32 i = 0; i < PlayerConfigs.Num(); ++i)
	{
		ScoreLines.Add(FString::Printf(TEXT("%s: %i"), *PlayerConfigs[i].Name, GetPlayerScore(i)));
	}

	return FString::Join(ScoreLines, TEXT("\n"));
}

TArray<FPlayerConfiguration> UDynamoGameInstance::GetPlayerConfig()
{
	return PlayerConfigs;
}
