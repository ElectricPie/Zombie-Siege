// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameHudWidget.generated.h"

class ATopDownPlayerController;
class UPlayerMoneyWidget;
class UAmmoCounterWidget;
class AGun;
class APlayerCharacter;
class UTextBlock;

/**
 * 
 */
UCLASS(Abstract)
class UGameHudWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Setup(ATopDownPlayerController* PlayerController, APlayerCharacter* PlayerCharacter);
	void UpdateInteractText(FText const & InteractText);
	void ShowInteractText(bool bShowInteractText);
	void UpdateRoundNumber(int32 RoundNumber);
	
private:
	UFUNCTION()
	void OnAmmoChanged(int32 NewAmmoCount, int32 MaxAmmo);
	UFUNCTION()
	void OnWeaponChanged(AGun* NewWeapon);
	UFUNCTION()
	void OnWeaponReloadStateChanged(bool bIsReloading);
	UFUNCTION()
	void OnMoneyChanged(int32 NewMoneyAmount, int32 AmountChanged);
	
private:
	UPROPERTY(VisibleAnywhere, Category="Widgets", meta=(BindWidget))
	UTextBlock* InteractTextBlock;
	UPROPERTY(VisibleAnywhere, Category="Widgets", meta=(BindWidget))
	UAmmoCounterWidget* AmmoCounterWidget;
	UPROPERTY(VisibleAnywhere, Category="Widgets", meta=(BindWidget))
	UPlayerMoneyWidget* MoneyWidget;
	UPROPERTY(VisibleAnywhere, Category="Widgets", meta=(BindWidget))
	UTextBlock* RoundText;

	TWeakObjectPtr<AGun> CurrentWeapon;

	FDelegateHandle AmmoChangeHandle;
	FDelegateHandle WeaponReloadHandle;
};
