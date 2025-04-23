// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "NetworkingSystemLibrary.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESIEGE_API UNetworkingSystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
	static bool IsValidIPAddress(const FString& IpAddress);
};
