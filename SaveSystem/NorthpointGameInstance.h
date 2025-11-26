// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "NorthpointGameInstance.generated.h"

/**
 * Sets own Game Instance base for BP class
 */
UCLASS()
class NORTHPOINT_API UNorthpointGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;
};
