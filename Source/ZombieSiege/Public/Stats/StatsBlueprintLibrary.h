// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "StatsBlueprintLibrary.generated.h"

class UHealthWidgetController;
/**
 * 
 */
UCLASS()
class ZOMBIESIEGE_API UStatsBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintPure, Category="WeaponSystemLibrary|WidgetController", meta=(DefaultToSelf="WorldContextObject"))
	static UHealthWidgetController* GetHealthWidgetController(const UObject* WorldContextObject);
};
