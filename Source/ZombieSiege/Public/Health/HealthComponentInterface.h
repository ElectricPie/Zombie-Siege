// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HealthComponentInterface.generated.h"

class UHealthComponent;

// This class does not need to be modified.
UINTERFACE()
class UHealthComponentInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ZOMBIESIEGE_API IHealthComponentInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent)
	UHealthComponent* GetHealthComponent() const;
	
};
