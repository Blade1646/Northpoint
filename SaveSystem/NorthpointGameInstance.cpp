// Fill out your copyright notice in the Description page of Project Settings.

#include "NorthpointGameInstance.h"
#include "SaveManager.h"

void UNorthpointGameInstance::Init()
{
	Super::Init();

	USaveManager::Init();
}
