// ‎Copyright© 2016 UE4Arch.com

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DynamoGameModeBase.generated.h"

/**
 * 
 */
class ADynamoCharacter;
class ALevelManager;

UCLASS(Blueprintable, BlueprintType)
class DYNAMO_API ADynamoGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
	
public:
	void NotifyPlayerDied();
	void StartPlay() override;
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintPure, Category = "BombermanClone")
		float GetSecondsLeftInMatch() const;

	UPROPERTY(EditAnywhere)
		ALevelManager* LevelManager;

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "BombermanClone")
		void OnMatchEnded(ADynamoCharacter* Winner);

	UPROPERTY(EditAnywhere)
		float TimeLimitMinutes = 1.f;



private:
	bool  bIsMatchOver = false;
	float StartTime = 0.f;

	FTimerHandle MatchTimerHandle;

	void OnPostPlayerDiedTick();

	// Ends the match. Use a null winner for a draw.
	void EndMatch(ADynamoCharacter* const Winner);
};

