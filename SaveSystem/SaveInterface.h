// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SaveInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USaveInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class NORTHPOINT_API ISaveInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// Get a unique identifier for the actor
	UFUNCTION(BlueprintNativeEvent, Category = "SaveInterface")
	int64 GetUniqueID();

	// Set neccessary variables just before the actor is saved
	UFUNCTION(BlueprintNativeEvent, Category = "SaveInterface")
	void OnBeforeSave();
};
