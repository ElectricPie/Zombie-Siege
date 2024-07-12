// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerMoneyWidget.generated.h"

class UMoneyChangedWidget;
class UOverlay;
class UTextBlock;
/**
 * 
 */
UCLASS()
class UPlayerMoneyWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetMoneyText(const int32 MoneyAmount, const int32 AmountChanged = 0);
	
private:
	UPROPERTY(VisibleAnywhere, Category="Widgets", meta=(BindWidget))
	UOverlay* Overlay;
	UPROPERTY(VisibleAnywhere, Category="Widgets", meta=(BindWidget))
	UTextBlock* MoneyText;
	UPROPERTY(EditAnywhere)
	FString TextPrefix = TEXT("£");
	
	UPROPERTY(EditAnywhere, Category="Widgets")
	TSubclassOf<UMoneyChangedWidget> MoneyChangedWidgetClass;
};
