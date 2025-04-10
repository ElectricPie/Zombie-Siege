// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetController.h"
#include "Weapons/Gun.h"
#include "AmmoCounterWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUiAmmoChangedSignature, int32, CurrentAmmo, int32, MaxAmmo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUiReloadStateChangedSignature, bool, bIsReloading);

/**
 * 
 */
UCLASS()
class ZOMBIESIEGE_API UAmmoCounterWidgetController : public UWidgetController
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnUiAmmoChangedSignature AmmoChangedEvent;
	UPROPERTY(BlueprintAssignable)
	FOnUiReloadStateChangedSignature ReloadStateChangedEvent;
	
public:
	virtual void BindCallbackToDependencies() override;
	virtual void BroadcastInitialValues() override;

private:
	UPROPERTY()
	TObjectPtr<AGun> CurrentWeapon;
	FDelegateHandle WeaponReloadHandle;
	FDelegateHandle AmmoChangeHandle;
	
private:
	UFUNCTION()
	void OnWeaponChanged(AGun* NewWeapon);
};
