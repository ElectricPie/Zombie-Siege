// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GameHud.generated.h"

class UAmmoCounterWidgetController;
struct FWidgetControllerParams;
class UOverlayWidgetController;
class UZSiegeUserWidget;
class UOptionsWidget;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPauseMenuToggledSignature, bool /*bMenuIsOpen*/);

UENUM(BlueprintType)
enum EGameWidget
{
	GameHud UMETA(DisplayName = "Hud"),
	PauseMenu UMETA(DisplayName = "Menu"),
	PauseGameOptions UMETA(DisplayName = "Options")
};

class APlayerCharacter;
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

	UFUNCTION(BlueprintCallable)
	void SwitchActiveWidget(EGameWidget WidgetToActivate);

	UFUNCTION(BlueprintCallable)
	void ToggleMenu();

	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WidgetControllerParams);
	UAmmoCounterWidgetController* GetAmmoCounterWidgetController(const FWidgetControllerParams& WidgetControllerParams);
	
public:
	FOnPauseMenuToggledSignature OnPauseMenuToggledEvent;
	
private:
	UPROPERTY(EditAnywhere, Category="Widgets")
	TSubclassOf<UGameHudWidget> GameHudWidgetClass;
	UPROPERTY(EditAnywhere, Category="Widgets")
	TSubclassOf<UUserWidget> MenuWidgetClass;
	UPROPERTY(EditAnywhere, Category="Widgets")
	TSubclassOf<UUserWidget> GameOverWidgetClass;
	UPROPERTY(EditAnywhere, Category="Widgets")
	TSubclassOf<UOptionsWidget> OptionsWidgetClass;
	
	//TObjectPtr<UGameHudWidget> GameHudWidget;
	TObjectPtr<UUserWidget> MenuWidget;
	TObjectPtr<UUserWidget> GameOverWidget;
	TObjectPtr<UOptionsWidget> OptionsWidget;

	TArray<TWeakObjectPtr<UUserWidget>> Widgets;

	bool bMenuIsOpen = false;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UZSiegeUserWidget> OverlayWidgetClass;
	TObjectPtr<UZSiegeUserWidget> OverlayWidget;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAmmoCounterWidgetController> AmmoCounterWidgetControllerClass;
	TObjectPtr<UAmmoCounterWidgetController> AmmoCounterWidgetController;

private:	
	void CollapseAllWidgets();
	UFUNCTION()
	void OnOptionsClosed();
};
