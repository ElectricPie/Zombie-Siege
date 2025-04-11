// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

class UWeaponStatsDataAsset;
class UArrowComponent;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnAmmoChangedSignature, int32 /*NewAmmoCount*/, int32 /*MaxAmmo*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnReloadStateChanged, bool /*bIsReloading*/);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGunFiredSignature);

UCLASS()
class AGun : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable)
	FOnGunFiredSignature OnGunFiredEvent;
	
	FOnAmmoChangedSignature OnAmmoChangedEvent;
	FOnReloadStateChanged OnReloadStateChangedEvent;

public:	
	// Sets default values for this actor's properties
	AGun();

	UFUNCTION(BlueprintPure)
	const UWeaponStatsDataAsset* GetWeaponStats() const { return WeaponStats; }
	
	void StartFiring(AController* ShooterController, AActor* ShooterActor);
	void StopFiring();
	UFUNCTION(BlueprintCallable)
	void SetVisibility(bool bIsVisible);

	UFUNCTION(BlueprintPure, Category="Weapon")
	int32 GetCurrentAmmo() const { return CurrentAmmo; }
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void Reload();
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void CancelReload();
	UFUNCTION(BlueprintPure)
	bool GetIsReloading() const { return bIsReloading; }

	USkeletalMeshComponent* GetMesh() const { return GunMesh; }

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void SpawnProjectile(AController* ShooterController, AActor* ShooterActor);
	UFUNCTION()
	void SingleShot(AController* ShooterController, AActor* ShooterActor);
	UFUNCTION()
	void BurstShot(AController* ShooterController, AActor* ShooterActor);
	UFUNCTION()
	void FinishReload();

	void MagEmpty();
	
private:
	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<USceneComponent> Root;
	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<USkeletalMeshComponent> GunMesh;
	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<UArrowComponent> FiringArrow;
	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<USceneComponent> ProjectileSpawn;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UWeaponStatsDataAsset> WeaponStats;
	
	UPROPERTY(VisibleInstanceOnly, Category="Gun|Ammo")
	int32 CurrentAmmo = 0;
	
	bool bIsReloading = false;
	FTimerHandle ReloadingTimerHandle;
	
	bool bIsFiring = false;
	float LastFiredTime = 0.f;
	
	FTimerHandle ShotTimer;
	FTimerHandle BurstTimer;
	int32 BurstShotsFired = 0;
};
