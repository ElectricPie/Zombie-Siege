// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameSettingsDataAsset.generated.h"

class AGunBase;

USTRUCT(BlueprintType)
struct FPlayerCharacterSkin
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USkeletalMesh> CharacterMesh;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMesh> CharacterHeadGearMesh;
};

/**
 * 
 */
UCLASS()
class ZOMBIESIEGE_API UGameSettingsDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	int32 GetMaxPlayers() const { return MaxPlayers; }
	const TArray<FPlayerCharacterSkin>& GetPlayerStartMeshes() const { return PlayerCharacterSkins; }
	const TArray<TSubclassOf<AGunBase>>& GetStartingLoadout() const { return StartingLoadout; }

private:
	UPROPERTY(EditDefaultsOnly)	
	int32 MaxPlayers = 4;
	UPROPERTY(EditDefaultsOnly)
	TArray<FPlayerCharacterSkin> PlayerCharacterSkins;
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<AGunBase>> StartingLoadout; 
};
