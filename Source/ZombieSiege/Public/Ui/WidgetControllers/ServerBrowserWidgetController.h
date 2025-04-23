// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetController.h"
#include "ServerBrowserWidgetController.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESIEGE_API UServerBrowserWidgetController : public UWidgetController
{
	GENERATED_BODY()

	virtual void BindCallbackToDependencies() override;
	virtual void BroadcastInitialValues() override;
};
