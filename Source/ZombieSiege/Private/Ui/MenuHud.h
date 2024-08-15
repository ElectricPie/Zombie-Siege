// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MenuHud.generated.h"

class UOptionsWidget;

UENUM(BlueprintType)
enum EMenuWidget
{
	MainMenu UMETA(DisplayName = "Main Menu"),
	MainOptions UMETA(DisplayName = "Options")
};

/**
 * 
 */
UCLASS(Abstract)
class AMenuHud : public AHUD
{
	GENERATED_BODY()

public:
	UFUNCTION(BLueprintCallable)
	void SwitchActiveWidget(EMenuWidget WidgetToActivate);

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(BlueprintReadOnly, Category="Widgets")
	TObjectPtr<UUserWidget> MenuWidget;
	UPROPERTY(BLueprintReadOnly, Category="Widgets")
	TObjectPtr<UOptionsWidget> OptionsWidget;

private:
	UFUNCTION()
	void OnOptionsClosed();
	
private:
	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;
	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UOptionsWidget> OptionsWidgetClass;

	TArray<TWeakObjectPtr<UUserWidget>> Widgets;
};
