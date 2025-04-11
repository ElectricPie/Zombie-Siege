// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ZSiegeInteractionSystemLibrary.generated.h"

class UInteractionWidgetController;
/**
 * 
 */
UCLASS()
class ZOMBIESIEGE_API UZSiegeInteractionSystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category="InteractionSystemLibrary|WidgetController", meta=(DefaultToSelf="WorldContextObject"))
	static UInteractionWidgetController* GetInteractionWidgetController(const UObject* WorldContextObject);
};
