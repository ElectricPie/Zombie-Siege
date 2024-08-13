// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MenuHud.generated.h"

UENUM(BlueprintType)
enum EMenuWidget
{
	MainMenu UMETA(DisplayName = "Main Menu"),
	Options UMETA(DisplayName = "Options")
};

/**
 * 
 */
UCLASS(Abstract)
class AMenuHud : public AHUD
{
	GENERATED_BODY()

public:
	// UFUNCTION(BLueprintCallable)
	// void SwitchHud(EMenuWidget WidgetToDisplay);

protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;
	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UUserWidget> OptionsWidgetClass;

	TObjectPtr<UUserWidget> MenuWidget;
	TObjectPtr<UUserWidget> OptionsWidget;
};
