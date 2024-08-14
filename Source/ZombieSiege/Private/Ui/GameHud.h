// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GameHud.generated.h"

UENUM(BlueprintType)
enum EGameWidget
{
	Hud UMETA(DisplayName = "Hud"),
	Menu UMETA(DisplayName = "Menu")
};


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
	void ShowGameOver();

	void SwitchActiveWidget(EGameWidget WidgetToActivate);

private:
	void OnRoundChanged(int32 RoundNumber);
	
private:
	UPROPERTY(EditAnywhere, Category="Widgets")
	TSubclassOf<UGameHudWidget> GameHudWidgetClass;
	UPROPERTY(EditAnywhere, Category="Widgets")
	TSubclassOf<UUserWidget> MenuWidgetClass;
	UPROPERTY(EditAnywhere, Category="Widgets")
	TSubclassOf<UUserWidget> GameOverWidgetClass;
	
	UPROPERTY(VisibleAnywhere, Category="Widgets")
	TObjectPtr<UGameHudWidget> GameHudWidget;
	UPROPERTY(VisibleAnywhere, Category="Widgets")
	TObjectPtr<UUserWidget> MenuWidget;
	UPROPERTY(VisibleAnywhere, Category="Widgets")
	TObjectPtr<UUserWidget> GameOverWidget;

	TArray<TWeakObjectPtr<UUserWidget>> Widgets; 
};
