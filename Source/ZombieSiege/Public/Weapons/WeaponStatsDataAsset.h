// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WeaponStatsDataAsset.generated.h"

class UFMODEvent;
class AGunProjectile;
class UDamageType;


UENUM(BlueprintType)
enum class EGunType : uint8
{
	Pistol UMETA(DisplayName="Pistol"),
	Rifle UMETA(DisplayName="Rifle")
};

UENUM(BlueprintType)
enum class EGunFireMode : uint8
{
	Single UMETA(DisplayName="Single"),
	Burst UMETA(DisplayName="Burst"),
	FullAuto UMETA(DisplayName="Full Auto")
};

/**
 * 
 */
UCLASS()
class ZOMBIESIEGE_API UWeaponStatsDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
	int32 GetMaxAmmo() const { return MaxAmmo; }
	
	UFUNCTION(BlueprintPure)
	TSubclassOf<AGunProjectile> GetProjectileClass() const { return ProjectileClass; }
	UFUNCTION(BlueprintPure)
	TSubclassOf<UDamageType> GetProjectileDamageType() const { return ProjectileDamageType; }
	UFUNCTION(BlueprintPure)
	float GetProjectileDamage() const { return ProjectileDamage; }

	UFUNCTION(BlueprintPure)
	EGunType GetGunType() const { return Type; }
	UFUNCTION(BlueprintPure)
	EGunFireMode GetFireMode() const { return FireMode; }
	UFUNCTION(BlueprintPure)
	float GetFireCooldownTime() const { return FireCooldownTime; }
	UFUNCTION(BlueprintPure)
	float GetShotIntervals() const { return ShotIntervals; }
	UFUNCTION(BlueprintPure)
	int32 GetBurstShots() const { return BurstShots; }
	UFUNCTION(BlueprintPure)
	UAnimMontage* GetReloadAnimMontage() const { return ReloadMontage; }
	UFUNCTION(BlueprintPure)
	float GetReloadTime() const { return ReloadTime; }

	UFUNCTION(BlueprintPure)
	UFMODEvent* GetFireSound() const { return FireSound; }
	UFUNCTION(BlueprintPure)
	UFMODEvent* GetReloadSound() const { return ReloadSound; }
	UFUNCTION(BlueprintPure)
	UFMODEvent* GetEmptySound() const { return EmptySound; }

private:
	UPROPERTY(EditDefaultsOnly, Category="Ammo")
	int32 MaxAmmo = 30;

	UPROPERTY(EditAnywhere, Category="Projectile")
	TSubclassOf<AGunProjectile> ProjectileClass;
	UPROPERTY(EditAnywhere, Category="Projectile")
	TSubclassOf<UDamageType> ProjectileDamageType = UDamageType::StaticClass();
	UPROPERTY(EditAnywhere, Category="Projectile", meta=(ClampMin=0.f, UIMin=0.f))
	float ProjectileDamage = 20.f;

	UPROPERTY(EditDefaultsOnly)
	EGunType Type;
	UPROPERTY(EditDefaultsOnly, Category="FireRate")
	EGunFireMode FireMode;
	UPROPERTY(EditDefaultsOnly, Category="FireRate", meta=(ClampMin=0.1f, UIMin=0.1f, ToolTip="The time in seconds before the gun can be fired again after firing has started"))
	float FireCooldownTime = 1.f;
	UPROPERTY(EditDefaultsOnly, Category="FireRate", meta=(ClampMin=0.1f, UIMin=0.1f, EditCondition="FireMode!=EGunFireMode::Single", ToolTip="The time between shots when Fire Rate is set to full auto or Burst"))
	float ShotIntervals = 1.f;
	UPROPERTY(EditDefaultsOnly, Category="FireRate", meta=(ClampMin=1, UIMin=1, EditCondition="FireMode==EGunFireMode::Burst", ToolTip="The number of shots in a burst when Fire Rate is set to Burst"))
	int32 BurstShots = 3;
	
	UPROPERTY(EditDefaultsOnly, Category="Animations")
	float ReloadTime = 2.f;
	UPROPERTY(EditDefaultsOnly, Category="Animations")
	TObjectPtr<UAnimMontage> ReloadMontage;

	UPROPERTY(EditDefaultsOnly, Category="Audio")
	TObjectPtr<UFMODEvent> FireSound;
	UPROPERTY(EditDefaultsOnly, Category="Audio")
	TObjectPtr<UFMODEvent> ReloadSound;
	UPROPERTY(EditDefaultsOnly, Category="Audio")
	TObjectPtr<UFMODEvent> EmptySound;
};
