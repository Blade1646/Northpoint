// Fill out your copyright notice in the Description page of Project Settings.

#include "NorthpointGameModeBase.h"
#include "SaveManager.h"

APawn* ANorthpointGameModeBase::SpawnDefaultPawnAtTransform_Implementation(
	AController* NewPlayer,
	const FTransform& SpawnTransform
)
{
	// Let the base game mode spawn the pawn
	APawn* Pawn = Super::SpawnDefaultPawnAtTransform_Implementation(NewPlayer, SpawnTransform);

	// Load game after that
	USaveManager::LoadGame();

	return Pawn;
}