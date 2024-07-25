// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponLoadoutComponent.generated.h"

class AGun;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponChangedSignature, AGun*, NewWeapon);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnWeaponAddedSignature, AGun*);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UWeaponLoadoutComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponLoadoutComponent();

	UFUNCTION(BlueprintCallable)
	void AddWeapon(AGun* Weapon, bool bEquip = false);
	UFUNCTION(BlueprintCallable)
	AGun* GetEquippedWeapon();
	UFUNCTION()
	void EquipNextWeapon();
	int32 GetWeaponCount() const { return Weapons.Num(); }

public:
	UPROPERTY(BlueprintAssignable, Category=Weapon)
	FOnWeaponChangedSignature OnWeaponChangedEvent;
	FOnWeaponAddedSignature OnWeaponAddedEvent;

protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditAnywhere, Category="Weapon", meta=(ClampMin=0, UIMin=0))
	int32 EquippedWeaponIndex;
	UPROPERTY(VisibleAnywhere, Category="Weapon")
	TArray<TWeakObjectPtr<AGun>> Weapons;
};
