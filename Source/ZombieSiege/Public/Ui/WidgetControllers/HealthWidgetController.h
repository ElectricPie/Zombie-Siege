// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetController.h"
#include "HealthWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUiHealthPercentageChangedSignature, const float, NewHealthPercentage);

/**
 * 
 */
UCLASS()
class ZOMBIESIEGE_API UHealthWidgetController : public UWidgetController
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnUiHealthPercentageChangedSignature OnHealthPercentageChangedEvent;
	
public:
	virtual void BindCallbackToDependencies() override;
	virtual void BroadcastInitialValues() override;
};
