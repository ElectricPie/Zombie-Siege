// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WeaponBuyPointDataAsset.generated.h"

class AGun;
/**
 * 
 */
UCLASS()
class ZOMBIESIEGE_API UWeaponBuyPointDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
	TSubclassOf<AGun> GetWeaponClass() const { return WeaponClass; }
	UFUNCTION(BlueprintPure)
	int32 GetCost() const { return Cost; }
	
private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGun> WeaponClass;
	UPROPERTY(EditDefaultsOnly, meta=(ClampMin=0, UIMin=0))
	int32 Cost = 100;
};
