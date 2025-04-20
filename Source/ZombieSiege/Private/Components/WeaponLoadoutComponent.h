// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponLoadoutComponent.generated.h"

class AGunBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponChangedSignature, AGunBase*, NewWeapon);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnWeaponAddedSignature, AGunBase*);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UWeaponLoadoutComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponLoadoutComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;

	void Fire();
	void StopFiring();
	bool Reload();
	
	UFUNCTION(BlueprintCallable)
	void AddWeapon_Server(AGunBase* NewWeapon, bool bEquip = false);
	UFUNCTION(BlueprintCallable)
	AGunBase* GetEquippedWeapon();
	UFUNCTION(Server, Reliable)
	void ServerEquipNextWeapon();
	int32 GetWeaponCount() const { return Weapons.Num(); }

public:
	UPROPERTY(BlueprintAssignable, Category="Weapon Loadout")
	FOnWeaponChangedSignature OnWeaponChangedEvent;
	FOnWeaponAddedSignature OnWeaponAddedEvent;

protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(ReplicatedUsing=OnRep_EquippedWeaponIndex, EditAnywhere, Category="Weapon Loadout", meta=(ClampMin=0, UIMin=0))
	int32 EquippedWeaponIndex = -1;
	UPROPERTY(ReplicatedUsing=OnRep_Weapons, VisibleAnywhere, Category="Weapon Loadout")
	TArray<TObjectPtr<AGunBase>> Weapons;

private:
	UFUNCTION()
	void OnRep_EquippedWeaponIndex(const int32 OldEquippedWeaponIndex);
	UFUNCTION()
	void OnRep_Weapons();
};
