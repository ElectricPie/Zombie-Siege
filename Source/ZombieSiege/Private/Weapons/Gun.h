// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

class UArrowComponent;
class ATopDownPlayerController;
class AGunProjectile;

UENUM(BlueprintType)
enum EGunType
{
	Pistol UMETA(DisplayName=Pistol),
	Rifle UMETA(DisplayName=Rifle)
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
	
	void Fire(ATopDownPlayerController* Shooter, FVector FireDirection);
	UFUNCTION(BlueprintCallable)
	void SetVisibility(bool bIsVisible);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, Category=Components)
	USceneComponent* Root;
	UPROPERTY(VisibleAnywhere, Category=Components)
	USkeletalMeshComponent* GunMesh;
	UPROPERTY(VisibleAnywhere, Category=Components)
	UArrowComponent* FiringArrow;
	
	UPROPERTY(VisibleAnywhere, Category=Projectile)
	USceneComponent* ProjectileSpawn;
	UPROPERTY(EditAnywhere, Category=Projectile)
	TSubclassOf<AGunProjectile> ProjectileClass;
	UPROPERTY(EditAnywhere, Category=Projectile)
	TSubclassOf<UDamageType> ProjectileDamageType = UDamageType::StaticClass();
	UPROPERTY(EditAnywhere, Category=Projectile, meta=(ClampMin=0.f, UIMin=0.f))
	float ProjectileDamage = 20.f;

	UPROPERTY(EditDefaultsOnly, Category=Weapon)
	TEnumAsByte<EGunType> Type;
};
