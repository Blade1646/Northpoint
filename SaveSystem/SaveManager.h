// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SaveMetadata.h"
#include "SaveInterface.h"
#include "UObject/NoExportTypes.h"
#include "SaveManager.generated.h"
 
/**
 * The manager class of the save system
 */
UCLASS()
class NORTHPOINT_API USaveManager : public UObject
{
    GENERATED_BODY()

private:
    // The current save slot
    static FString CurrentSaveSlot;

public:
    USaveManager();
    ~USaveManager();

    // Initialize the class. Must be called when the game first launches
    static void Init();

    // Queries all the actors in the game that implement the save interface
    static void QueryAllSaveInterfaces();

    // Saves the current state of the game
    UFUNCTION(BlueprintCallable, Category = "SaveAndLoad")
    static void SaveGame();

    // Loads the current state of the game
    UFUNCTION(BlueprintCallable, Category = "SaveAndLoad")
    static void LoadGame();

    // Deletes the specified slot
    UFUNCTION(BlueprintCallable, Category = "SaveAndLoad")
    static void DeleteSlot(const FString& slot);

    // Gets a new, unused slot
    UFUNCTION(BlueprintPure, Category = "SaveAndLoad")
    static UPARAM(DisplayName = "Slot") FString GetNewSaveSlot(bool& slot_found);

    // Sets the current save slot
    UFUNCTION(BlueprintCallable, Category = "SaveAndLoad")
    static void SetCurrentSaveSlot(const FString& slot);

    // Gets the current save slot
    UFUNCTION(BlueprintPure, Category = "SaveAndLoad")
    static FString GetCurrentSaveSlot();

    // Gets all the saved games
    UFUNCTION(BlueprintPure, Category = "SaveAndLoad")
    static TArray<FSaveMetadata> GetAllSaveMetadata();
};
