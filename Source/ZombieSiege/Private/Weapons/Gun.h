// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

class UArrowComponent;
class ATopDownPlayerController;
class AGunProjectile;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnAmmoChangedSignature, int32 /*NewAmmoCount*/, int32 /*MaxAmmo*/);

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

	FOnAmmoChangedSignature OnAmmoChangedEvent;

	UFUNCTION(BlueprintPure)
	EGunType GetGunType() const { return Type; }
	
	void Fire(ATopDownPlayerController* Shooter);
	void StopFiring();
	UFUNCTION(BlueprintCallable)
	void SetVisibility(bool bIsVisible);

	UFUNCTION(BlueprintPure, Category="Weapon")
	int32 GetMaxAmmo() const { return MaxAmmo; }
	UFUNCTION(BlueprintPure, Category="Weapon")
	int32 GetCurrentAmmo() const { return CurrentAmmo; }
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, Category="Components")
	USceneComponent* Root;
	UPROPERTY(VisibleAnywhere, Category="Components")
	USkeletalMeshComponent* GunMesh;
	UPROPERTY(VisibleAnywhere, Category="Components")
	UArrowComponent* FiringArrow;
	
	UPROPERTY(EditDefaultsOnly, Category="Ammo", meta=(ClampMin=1, UIMin=1))
	uint32 MaxAmmo = 30;
	UPROPERTY(VisibleAnywhere, Category="Ammo", meta=(ClampMin=1, UIMin=1))
	uint32 CurrentAmmo = 0;	
	
	UPROPERTY(VisibleAnywhere, Category="Projectile")
	USceneComponent* ProjectileSpawn;
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
	
	bool bIsFiring = false;
	float LastFiredTime = 0.f;
	
	FTimerHandle ShotTimer;
	FTimerHandle BurstTimer;
	int32 BurstShotsFired = 0;
	
	UFUNCTION()
	void SpawnProjectile(ATopDownPlayerController* Shooter);
	UFUNCTION()
	void SingleShot(ATopDownPlayerController* Shooter);
	UFUNCTION()
	void BurstShot(ATopDownPlayerController* Shooter);
};
