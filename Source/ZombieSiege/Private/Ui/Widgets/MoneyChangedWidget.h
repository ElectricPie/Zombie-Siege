// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MoneyChangedWidget.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS(Abstract)
class UMoneyChangedWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetAmount(const int32 Amount); 

private:
	UPROPERTY(VisibleAnywhere, meta=(BindWidget))
	UTextBlock* MoneyTextBlock;
	UPROPERTY(EditAnywhere)
	FColor PositiveColor = FColor::Green;
	UPROPERTY(EditAnywhere)
	FColor NegativeColor = FColor::Red;
	
};
