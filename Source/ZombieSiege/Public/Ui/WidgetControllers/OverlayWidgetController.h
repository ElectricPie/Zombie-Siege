// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetController.h"
#include "OverlayWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUiRoundChangedSignature, const int32, NewRound);

/**
 * 
 */
UCLASS()
class ZOMBIESIEGE_API UOverlayWidgetController : public UWidgetController
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnUiRoundChangedSignature OnRoundChangedEvent;

public:
	virtual void BindCallbackToDependencies() override;
	virtual void BroadcastInitialValues() override;
};
