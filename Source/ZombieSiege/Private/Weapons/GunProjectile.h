// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GunProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

USTRUCT(BlueprintType)
struct FGunProjectileInitData
{
	GENERATED_BODY()

	FGunProjectileInitData() {}
	FGunProjectileInitData(AController* InController, AActor* InActor, TSubclassOf<UDamageType> InDamageType, const float InDamage)
		: Controller(InController), Actor(InActor), DamageType(InDamageType), Damage(InDamage) {}
	
	TWeakObjectPtr<AController> Controller = nullptr;
	TWeakObjectPtr<AActor> Actor = nullptr;
	TSubclassOf<UDamageType> DamageType = nullptr;
	float Damage = 0.f;
};

UCLASS(Abstract)
class AGunProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGunProjectile();

	void InitProjectile(const FGunProjectileInitData& InitData);
	// void Init(AController* Controller, AActor* Actor, TSubclassOf<UDamageType> NewDamageType, float NewDamage);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
private:
	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<USphereComponent> CollisionComponent;
	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;
	
	float Damage;
	TWeakObjectPtr<AController> ShooterController = nullptr;
	TWeakObjectPtr<AActor> ShooterActor = nullptr;
	TSubclassOf<UDamageType> DamageType;
	
	UPROPERTY(EditAnywhere, Category="Projectile")
	float DestroyTime = 5.f;
};
