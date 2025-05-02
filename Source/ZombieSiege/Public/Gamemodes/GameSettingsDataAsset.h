// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameSettingsDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESIEGE_API UGameSettingsDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	int32 GetMaxPlayers() const { return MaxPlayers; }
	TArray<USkeletalMesh*> GetPlayerStartMeshes() const { return PlayerStartMeshes; }

private:
	UPROPERTY(EditDefaultsOnly)	
	int32 MaxPlayers = 4;
	UPROPERTY(EditDefaultsOnly)
	TArray<TObjectPtr<USkeletalMesh>> PlayerStartMeshes;
};
