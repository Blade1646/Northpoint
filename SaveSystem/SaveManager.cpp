// Fill out your copyright notice in the Description page of Project Settings.

#include "SaveManager.h"
#include "SaveGameMetadata.h"
#include "SaveGameData.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"

FString USaveManager::CurrentSaveSlot;

// Filename for metadata save slot
static const FString kMetadataSaveSlot = "SaveGameMetadata";
// Max amount of save slots
static const int32 kMaxSaveSlots = 100;

USaveManager::USaveManager()
{
}

USaveManager::~USaveManager()
{
}

void USaveManager::Init()
{
    CurrentSaveSlot = "Default";

    // Make sure the metadata file exists if the game has never been ran
    USaveGame* saveGameMetadata = UGameplayStatics::LoadGameFromSlot(kMetadataSaveSlot, 0);

    if (saveGameMetadata == nullptr)
    {
        // Since the metadata file doesn't exist, we need to create one
        USaveGame* saveGameObject = UGameplayStatics::CreateSaveGameObject(USaveGameMetadata::StaticClass());

        UGameplayStatics::SaveGameToSlot(saveGameObject, kMetadataSaveSlot, 0);
    }
}

void USaveManager::SaveGame()
{
    // Create a new save game data instance
    USaveGameData* saveGameData = Cast<USaveGameData>(UGameplayStatics::CreateSaveGameObject(USaveGameData::StaticClass()));

    // Go over all the actors that need to be saved and save them
    TArray<AActor*> actors;
    UGameplayStatics::GetAllActorsWithInterface(GWorld, USaveInterface::StaticClass(), actors);
    for (AActor* actor : actors)
    {
        if (!IsValid(actor))
            continue;

        // Notify the object that it's about to be saved
        ISaveInterface::Execute_OnBeforeSave(actor);

        // Find and add the saving object by its ID
        int64 uniqueID = ISaveInterface::Execute_GetUniqueID(actor);
        FSaveData& saveData = saveGameData->SerializedData.Add(uniqueID);

        // Write corresponding save data of the object
        FMemoryWriter memoryWriter = FMemoryWriter(saveData.Data);
        memoryWriter.ArIsSaveGame = true;

        actor->Serialize(memoryWriter);
    }

    // Save the game to the current slot
    UGameplayStatics::SaveGameToSlot(saveGameData, CurrentSaveSlot, 0);

    // Update the metadata file with the new slot
    USaveGameMetadata* saveGameMetadata = Cast<USaveGameMetadata>(UGameplayStatics::LoadGameFromSlot(kMetadataSaveSlot, 0));

    FSaveMetadata& saveMetadata = saveGameMetadata->SavedGamesMetadata.FindOrAdd(CurrentSaveSlot);
    saveMetadata.SlotName = CurrentSaveSlot;
    saveMetadata.Date = FDateTime::Now();

    // Save the changes to the metadata file
    UGameplayStatics::SaveGameToSlot(saveGameMetadata, kMetadataSaveSlot, 0);
}

void USaveManager::LoadGame()
{
    // Load game from the current slot to the save game object
    USaveGameData* saveGameData = Cast<USaveGameData>(UGameplayStatics::LoadGameFromSlot(CurrentSaveSlot, 0));

    // For the first launch
    if (saveGameData == nullptr)
    {
        // No saves exist yet for this slot. Save a default one.
        SaveGame();

        // Reload it
        saveGameData = Cast<USaveGameData>(UGameplayStatics::LoadGameFromSlot(CurrentSaveSlot, 0));
    }

    // Loop over all the actors that need to load data and load their data
    TArray<AActor*> actors;
    UGameplayStatics::GetAllActorsWithInterface(GWorld, USaveInterface::StaticClass(), actors);
    for (AActor* actor : actors)
    {
        if (!IsValid(actor))
            continue;

        // Find the save data of the object by its ID
        int64 uniqueID = ISaveInterface::Execute_GetUniqueID(actor);
        FSaveData* saveData = saveGameData->SerializedData.Find(uniqueID);

        if (saveData == nullptr)
            continue;

        // Deserialize object's save data
        FMemoryReader memoryReader(saveData->Data);
        memoryReader.ArIsSaveGame = false;

        actor->Serialize(memoryReader);
    }
}

void USaveManager::DeleteSlot(const FString& slot)
{
    // Delete the slot with parameter name
    UGameplayStatics::DeleteGameInSlot(slot, 0);

    // Loading the metadata from file
    USaveGameMetadata* saveGameMetadata = Cast<USaveGameMetadata>(UGameplayStatics::LoadGameFromSlot(kMetadataSaveSlot, 0));
    
    // Update the metadata save game object
    saveGameMetadata->SavedGamesMetadata.Remove(slot);

    // Save the metadata to its slot
    UGameplayStatics::SaveGameToSlot(saveGameMetadata, kMetadataSaveSlot, 0);
}

FString USaveManager::GetNewSaveSlot(bool& slot_found)
{
    slot_found = false;

    // Loading the metadata from file
    USaveGameMetadata* saveGameMetadata = Cast<USaveGameMetadata>(UGameplayStatics::LoadGameFromSlot(kMetadataSaveSlot, 0));

    // Iterate over all possible slots
    for (int32 i = 0; i < kMaxSaveSlots; ++i)
    {
        // SaveSlot0, SaveSlot1, SaveSlot2, etc...
        FString slotName = "SaveSlot" + FString::FromInt(i);

        // Verifying if metadata does not contain slot with given name
        if (saveGameMetadata->SavedGamesMetadata.Contains(slotName) == false)
        {
            // Return the found slot
            slot_found = true;
            return slotName;
        }
    }

    return FString();
}

void USaveManager::SetCurrentSaveSlot(const FString& slot)
{
    CurrentSaveSlot = slot;
}

FString USaveManager::GetCurrentSaveSlot()
{
	return CurrentSaveSlot;
}

TArray<FSaveMetadata> USaveManager::GetAllSaveMetadata()
{
    TArray<FSaveMetadata> metadata;
	
    // Loading the metadata from file
    USaveGameMetadata* saveGameMetadata = Cast<USaveGameMetadata>(UGameplayStatics::LoadGameFromSlot(kMetadataSaveSlot, 0));

    metadata.Reserve(saveGameMetadata->SavedGamesMetadata.Num());

    // Add each saved game's metadata to the return array
    for (const auto& saved_game : saveGameMetadata->SavedGamesMetadata)
    {
        metadata.Push(saved_game.Value);
    }

    return metadata;
}