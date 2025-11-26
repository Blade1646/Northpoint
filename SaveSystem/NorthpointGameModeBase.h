// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NorthpointGameModeBase.generated.h"

/**
 * Sets own Game Mode base for BP class
 */
UCLASS()
class NORTHPOINT_API ANorthpointGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual APawn* SpawnDefaultPawnAtTransform_Implementation(
		AController* NewPlayer,
		const FTransform& SpawnTransform
	) override;
};
