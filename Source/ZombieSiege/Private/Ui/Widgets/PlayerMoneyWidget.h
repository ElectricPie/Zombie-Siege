// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerMoneyWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class UPlayerMoneyWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetMoneyText(const int32 MoneyAmount) const;
	
private:
	UPROPERTY(VisibleAnywhere, meta=(BindWidget))
	UTextBlock* MoneyText;
	UPROPERTY(EditAnywhere)
	FString TextPrefix = TEXT("£");
};
