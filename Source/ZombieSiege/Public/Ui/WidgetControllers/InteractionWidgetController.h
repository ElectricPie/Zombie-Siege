// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetController.h"
#include "InteractionWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInteractableMessageSignature, bool, bShowMessage, FText, Message);

/**
 * 
 */
UCLASS()
class ZOMBIESIEGE_API UInteractionWidgetController : public UWidgetController
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnInteractableMessageSignature InteractionMessageEvent;
	
public:
	virtual void BindCallbackToDependencies() override;
};
