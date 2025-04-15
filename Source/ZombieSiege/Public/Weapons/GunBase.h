// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GunBase.generated.h"

class UWeaponStatsDataAsset;
class UArrowComponent;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnAmmoChangedSignature, int32 /*NewAmmoCount*/, int32 /*MaxAmmo*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnReloadStateChanged, bool /*bIsReloading*/);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGunFiredSignature);

UCLASS()
class AGunBase : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable)
	FOnGunFiredSignature OnGunFiredEvent;
	
	FOnAmmoChangedSignature OnAmmoChangedEvent;
	FOnReloadStateChanged OnReloadStateChangedEvent;

public:	
	// Sets default values for this actor's properties
	AGunBase();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintPure)
	const UWeaponStatsDataAsset* GetWeaponStats() const { return WeaponStats; }

	void Fire();
	void StopFiring();
	void Reload();
	
	UFUNCTION(BlueprintCallable)
	void SetVisibility(bool bIsVisible) const;

	UFUNCTION(BlueprintPure, Category="Weapon")
	int32 GetCurrentAmmo() const { return CurrentAmmo; }

	UFUNCTION(BlueprintPure)
	bool GetIsReloading() const { return bIsReloading; }

// 	USkeletalMeshComponent* GetMesh() const { return GunMesh; }

protected:
	virtual void BeginPlay() override;

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
	
	UPROPERTY(ReplicatedUsing=OnRep_CurrentAmmo, VisibleInstanceOnly, Category="Gun|Ammo")
	int32 CurrentAmmo = 0;
	UPROPERTY(Replicated)
	bool bIsReloading = false;
	FTimerHandle ReloadingTimerHandle;
	
	bool bIsFiring = false;
	float LastFiredTime = 0.f;
	
	FTimerHandle ShotTimer;
	FTimerHandle BurstTimer;
	int32 BurstShotsFired = 0;
	
private:
	UFUNCTION(Server, Reliable)
	void Fire_Server();
	UFUNCTION(Server, Reliable)
	void StopFiring_Server();
	UFUNCTION(Server, Reliable)
	void Reload_Server();
	UFUNCTION(Server, Reliable)
	void CancelReload_Server();
	
	UFUNCTION()
	void SpawnProjectile(AController* ShooterController, AActor* ShooterActor);
	UFUNCTION()
	void SingleShot(AController* ShooterController, AActor* ShooterActor);
	UFUNCTION()
	void BurstShot(AController* ShooterController, AActor* ShooterActor);
	
	void HandleFireMode(AController* ShooterController, AActor* ShooterActor);

	void MagEmpty();

	UFUNCTION()
	void OnRep_CurrentAmmo() const;

	bool CanReload() const;
};
