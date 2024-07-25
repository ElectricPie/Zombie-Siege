// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

class UArrowComponent;
class AGunProjectile;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnAmmoChangedSignature, int32 /*NewAmmoCount*/, int32 /*MaxAmmo*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnReloadStateChanged, bool /*bIsReloading*/);

UENUM(BlueprintType)
enum EGunType
{
	Pistol UMETA(DisplayName="Pistol"),
	Rifle UMETA(DisplayName="Rifle")
};

UENUM()
enum EGunFireRate
{
	Single UMETA(DisplayName="Single"),
	Burst UMETA(DisplayName="Burst"),
	FullAuto UMETA(DisplayName="Full Auto")
};

UCLASS()
class AGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGun();

	UFUNCTION(BlueprintPure)
	EGunType GetGunType() const { return Type; }
	
	void StartFiring(AController* ShooterController, AActor* ShooterActor);
	void StopFiring();
	UFUNCTION(BlueprintCallable)
	void SetVisibility(bool bIsVisible);

	UFUNCTION(BlueprintPure, Category="Weapon")
	int32 GetMaxAmmo() const { return MaxAmmo; }
	UFUNCTION(BlueprintPure, Category="Weapon")
	int32 GetCurrentAmmo() const { return CurrentAmmo; }
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void Reload();
	UFUNCTION(BlueprintPure)
	UAnimMontage* GetReloadAnimMontage() const { return ReloadMontage; }
	UFUNCTION(BlueprintPure)
	bool GetIsReloading() { return bIsReloading; }

public:
	FOnAmmoChangedSignature OnAmmoChangedEvent;
	FOnReloadStateChanged OnReloadStateChangedEvent;

protected:
	// Called when the game starts or when spawned
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
	
private:
	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<USceneComponent> Root;
	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<USkeletalMeshComponent> GunMesh;
	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<UArrowComponent> FiringArrow;
	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<USceneComponent> ProjectileSpawn;
	
	UPROPERTY(EditDefaultsOnly, Category="Ammo", meta=(ClampMin=1, UIMin=1))
	uint32 MaxAmmo = 30;
	UPROPERTY(VisibleAnywhere, Category="Ammo", meta=(ClampMin=1, UIMin=1))
	uint32 CurrentAmmo = 0;	
	
	UPROPERTY(EditAnywhere, Category="Projectile")
	TSubclassOf<AGunProjectile> ProjectileClass;
	UPROPERTY(EditAnywhere, Category="Projectile")
	TSubclassOf<UDamageType> ProjectileDamageType = UDamageType::StaticClass();
	UPROPERTY(EditAnywhere, Category="Projectile", meta=(ClampMin=0.f, UIMin=0.f))
	float ProjectileDamage = 20.f;

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	TEnumAsByte<EGunType> Type;
	
	UPROPERTY(EditDefaultsOnly, Category="FireRate")
	TEnumAsByte<EGunFireRate> FireRate;
	UPROPERTY(EditDefaultsOnly, Category="FireRate", meta=(ClampMin=0.1f, UIMin=0.1f, ToolTip="The time in seconds before the gun can be fired again after firing has started"))
	float FireCooldownTime = 1.f;
	UPROPERTY(EditDefaultsOnly, Category="FireRate", meta=(ClampMin=0.1f, UIMin=0.1f, EditCondition="FireRate!=EGunFireRate::Single", ToolTip="The time between shots when Fire Rate is set to full auto or Burst"))
	float ShotIntervals = 1.f;
	UPROPERTY(EditDefaultsOnly, Category="FireRate", meta=(ClampMin=1, UIMin=1, EditCondition="FireRate==EGunFireRate::Burst", ToolTip="The number of shots in a burst when Fire Rate is set to Burst"))
	int32 BurstShots = 3;

	UPROPERTY(EditAnywhere, Category="Animation")
	UAnimMontage* ReloadMontage;
	UPROPERTY(EditAnywhere, Category="Animation", meta=(ToolTip="The time a reload takes if the there is no reload animation"))
	float DefaultReloadTime = 2.f;
	bool bIsReloading = false;
	FTimerHandle ReloadingTimerHandle;
	
	bool bIsFiring = false;
	float LastFiredTime = 0.f;
	
	FTimerHandle ShotTimer;
	FTimerHandle BurstTimer;
	int32 BurstShotsFired = 0;
};
