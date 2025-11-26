// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveGameData.generated.h"

USTRUCT()
struct FSaveData
{
	GENERATED_BODY()

	// Stores raw binary data 
	UPROPERTY(VisibleAnywhere)
	TArray<uint8> Data;
};

/**
 * Object that represents single SaveGame file content
 */
UCLASS()
class NORTHPOINT_API USaveGameData : public USaveGame
{
	GENERATED_BODY()
	
public:
	// Contains a dictionary of unique IDs and their binary data
	UPROPERTY(VisibleAnywhere)
	TMap<int64, FSaveData> SerializedData;
};
