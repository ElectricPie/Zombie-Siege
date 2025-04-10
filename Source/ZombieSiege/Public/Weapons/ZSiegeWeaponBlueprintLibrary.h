// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ZSiegeWeaponBlueprintLibrary.generated.h"

class UAmmoCounterWidgetController;

/**
 * 
 */
UCLASS()
class ZOMBIESIEGE_API UZSiegeWeaponBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category="WeaponSystemLibrary|WidgetController", meta=(DefaultToSelf="WorldContextObject"))
	static UAmmoCounterWidgetController* GetAmmoCounterWidgetController(const UObject* WorldContextObject);
};
