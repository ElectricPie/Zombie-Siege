// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GameHud.generated.h"

class UGameHudWidget;
/**
 * 
 */
UCLASS(Abstract)
class AGameHud : public AHUD
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	void SetInteractText(FText const & InteractText);
	void HideInteractText();

private:
	void OnRoundChanged(int32 RoundNumber);
	
private:
	UPROPERTY(EditAnywhere, Category=Widgets)
	TSubclassOf<UGameHudWidget> GameHudWidgetClass;
	UPROPERTY(VisibleAnywhere, Category=Widgets)
	UGameHudWidget* GameHudWidget;
};
