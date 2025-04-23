// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MainMenuSystemLibrary.generated.h"

class UMainMenuWidgetController;
/**
 * 
 */
UCLASS()
class ZOMBIESIEGE_API UMainMenuSystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category="MainMenuSystemLibrary|WidgetController", meta=(DefaultToSelf="WorldContextObject"))
	static UMainMenuWidgetController* GetMainMenuWidgetController(const UObject* WorldContextObject);
};
