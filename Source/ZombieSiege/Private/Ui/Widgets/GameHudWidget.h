// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameHudWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS(Abstract)
class UGameHudWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateInteractText(FText const & InteractText);
	void ShowInteractText(bool bShowInteractText);
	
private:
	UPROPERTY(VisibleAnywhere, meta=(BindWidget))
	UTextBlock* InteractTextBlock;
};
