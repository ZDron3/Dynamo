// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "DynamoGameInstance.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType, Blueprintable)
struct FPlayerConfiguration
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FColor Color;
};


UCLASS(Blueprintable)
class DYNAMO_API UDynamoGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	void  IncrementPlayerScore(const int32 PlayerId);
	int32 GetPlayerScore(const int32 PlayerId) const;

	UFUNCTION(BlueprintPure, Category = "Dynamo")
		FString GetScoreString() const;
	

	UFUNCTION(BlueprintPure, Category = "Dynamo")
	TArray<FPlayerConfiguration> GetPlayerConfig();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FPlayerConfiguration> PlayerConfigs;

private:
	TMap<int32, int32> PlayerScores;

	mutable FString CachedScoreString;

	FString GenerateScoreString() const;
		
};

