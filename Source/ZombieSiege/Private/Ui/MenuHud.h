// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MenuHud.generated.h"

class UZSiegeUserWidget;
struct FWidgetControllerParams;
class UMainMenuWidgetController;
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

	UMainMenuWidgetController* GetMainMenuWidgetController(const FWidgetControllerParams& WidgetControllerParams);

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(BlueprintReadOnly, Category="Widgets")
	TObjectPtr<UZSiegeUserWidget> MenuWidget;
	UPROPERTY(BLueprintReadOnly, Category="Widgets")
	TObjectPtr<UOptionsWidget> OptionsWidget;

private:
	UFUNCTION()
	void OnOptionsClosed();
	
private:
	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UZSiegeUserWidget> MainMenuWidgetClass;
	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UOptionsWidget> OptionsWidgetClass;

	TArray<TWeakObjectPtr<UUserWidget>> Widgets;

	UPROPERTY(EditDefaultsOnly, Category="Widget Controller")
	TSubclassOf<UMainMenuWidgetController> MainMenuWidgetControllerClass;
	UPROPERTY()
	TObjectPtr<UMainMenuWidgetController> MainMenuWidgetController;
};
