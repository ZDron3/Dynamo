// Fill out your copyright notice in the Description page of Project Settings.

#include "DynamoLocalViewportClient.h"
#include "Engine/Engine.h"

bool UDynamoLocalViewportClient::InputKey(
	FViewport* Viewport, int32 ControllerId, FKey Key, EInputEvent EventType, float AmountDepressed, bool bGamepad)
{
	if (IgnoreInput() || bGamepad || Key.IsMouseButton())
	{
		return Super::InputKey(Viewport, ControllerId, Key, EventType, AmountDepressed, bGamepad);
	}
	else
	{
		UEngine* const Engine = GetOuterUEngine();
		int32 const    NumPlayers = Engine ? Engine->GetNumGamePlayers(this) : 0;
		bool           bRetVal = false;
		for (int32 i = 0; i < NumPlayers; i++)
		{
			bRetVal = Super::InputKey(Viewport, i, Key, EventType, AmountDepressed, bGamepad) || bRetVal;
		}

		return bRetVal;
	}
}
//UE4 Workaround for mulitple player using same input


