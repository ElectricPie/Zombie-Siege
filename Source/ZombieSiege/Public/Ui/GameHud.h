// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "WidgetControllers/HealthWidgetController.h"
#include "GameHud.generated.h"

class UInteractionWidgetController;
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

	void ShowGameOver();

	UFUNCTION(BlueprintCallable)
	void SwitchActiveWidget(EGameWidget WidgetToActivate);

	UFUNCTION(BlueprintCallable)
	void ToggleMenu();

	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WidgetControllerParams);
	UAmmoCounterWidgetController* GetAmmoCounterWidgetController(const FWidgetControllerParams& WidgetControllerParams);
	UInteractionWidgetController* GetInteractionWidgetController(const FWidgetControllerParams& WidgetControllerParams);
	UHealthWidgetController* GetHealthWidgetController(const FWidgetControllerParams& WidgetControllerParams);

public:
	FOnPauseMenuToggledSignature OnPauseMenuToggledEvent;
	
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

private:	
	void CollapseAllWidgets();
	UFUNCTION()
	void OnOptionsClosed();
};
