// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AmmoCounterWidget.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class UAmmoCounterWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateAmmoText(int32 CurrentAmmo, int32 MaxAmmo);
	
private:
	UPROPERTY(VisibleAnywhere, Category="Widgets", meta=(BindWidget))
	UTextBlock* AmmoTextBlock;
};
