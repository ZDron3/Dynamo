// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameViewportClient.h"
#include "DynamoLocalViewportClient.generated.h"

/**
 * 
 */
UCLASS()
class DYNAMO_API UDynamoLocalViewportClient : public UGameViewportClient
{
	GENERATED_BODY()
	
		// BEGIN THIRD PARTY CODE (UE4 Wiki at https://wiki.unrealengine.com/Local_Multiplayer_Tips )

		virtual bool InputKey(FViewport*  Viewport,
			int32       ControllerId,
			FKey        Key,
			EInputEvent EventType,
			float       AmountDepressed = 1.f,
			bool        bGamepad = false) override;
	// END THIRD PARTY CODE
	
	
};
