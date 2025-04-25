// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Ui/WidgetControllers/HealthWidgetController.h"
#include "GameHud.generated.h"

class APlayerCharacter;
class UAmmoCounterWidgetController;
class UGameHudWidget;
class UInteractionWidgetController;
class UOptionsWidget;
class UOverlayWidgetController;
class UZSiegeUserWidget;
struct FWidgetControllerParams;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPauseMenuToggledSignature, bool /*bMenuIsOpen*/);

UENUM(BlueprintType)
enum EGameWidget
{
	GameHud UMETA(DisplayName = "Hud"),
	PauseMenu UMETA(DisplayName = "Menu"),
	PauseGameOptions UMETA(DisplayName = "Options")
};

/**
 * 
 */
UCLASS(Abstract)
class AGameHud : public AHUD
{
	GENERATED_BODY()

public:
	FOnPauseMenuToggledSignature OnPauseMenuToggledEvent;
	
public:
	void ShowGameOver();

	UFUNCTION(BlueprintCallable)
	void SwitchActiveWidget(EGameWidget WidgetToActivate);

	UFUNCTION(BlueprintCallable)
	void ToggleMenu();

	void InitHud();

	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WidgetControllerParams);
	UAmmoCounterWidgetController* GetAmmoCounterWidgetController(const FWidgetControllerParams& WidgetControllerParams);
	UInteractionWidgetController* GetInteractionWidgetController(const FWidgetControllerParams& WidgetControllerParams);
	UHealthWidgetController* GetHealthWidgetController(const FWidgetControllerParams& WidgetControllerParams);

	/*
	 * Rebinds any widget controller dependencies the rely on player characters
	 */
	void RebindCharacterWidgetControllerDependencies() const;
	
private:
	UPROPERTY(EditAnywhere, Category="Widgets")
	TSubclassOf<UUserWidget> MenuWidgetClass;
	UPROPERTY(EditAnywhere, Category="Widgets")
	TSubclassOf<UUserWidget> GameOverWidgetClass;
	UPROPERTY(EditAnywhere, Category="Widgets")
	TSubclassOf<UOptionsWidget> OptionsWidgetClass;
	
	UPROPERTY()
	TObjectPtr<UUserWidget> MenuWidget;
	UPROPERTY()
	TObjectPtr<UUserWidget> GameOverWidget;
	UPROPERTY()
	TObjectPtr<UOptionsWidget> OptionsWidget;
	UPROPERTY()
	TArray<TWeakObjectPtr<UUserWidget>> Widgets;

	bool bMenuIsOpen = false;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UZSiegeUserWidget> OverlayWidgetClass;
	UPROPERTY()
	TObjectPtr<UZSiegeUserWidget> OverlayWidget;
	
	UPROPERTY(EditDefaultsOnly, Category="WidgetControllers")
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;
	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;
	UPROPERTY(EditDefaultsOnly, Category="WidgetControllers")
	TSubclassOf<UAmmoCounterWidgetController> AmmoCounterWidgetControllerClass;
	UPROPERTY()
	TObjectPtr<UAmmoCounterWidgetController> AmmoCounterWidgetController;
	UPROPERTY(EditDefaultsOnly, Category="WidgetControllers")
	TSubclassOf<UInteractionWidgetController> InteractionWidgetControllerClass;
	UPROPERTY()
	TObjectPtr<UInteractionWidgetController> InteractionWidgetController;
	UPROPERTY(EditDefaultsOnly, Category="WidgetControllers")
	TSubclassOf<UHealthWidgetController> HealthWidgetControllerClass;
	UPROPERTY()
	TObjectPtr<UHealthWidgetController> HealthWidgetController;

	bool bHasBeenInitialized = false;

private:	
	void CollapseAllWidgets();
	UFUNCTION()
	void OnOptionsClosed();
};
