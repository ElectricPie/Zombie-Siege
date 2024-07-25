// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GunProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS(Abstract)
class AGunProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGunProjectile();

	void Init(AController* Controller, AActor* Actor, TSubclassOf<UDamageType> NewDamageType, float NewDamage);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void DestroyProjectile();
	
private:
	UPROPERTY(VisibleAnywhere, Category="Components")
	USphereComponent* CollisionComponent;
	UPROPERTY(VisibleAnywhere, Category="Components")
	UStaticMeshComponent* ProjectileMesh;
	UPROPERTY(VisibleAnywhere, Category="Components")
	UProjectileMovementComponent* ProjectileMovementComponent;
	
	float Damage;
	TWeakObjectPtr<AController> ShooterController = nullptr;
	TWeakObjectPtr<AActor> ShooterActor = nullptr;
	TSubclassOf<UDamageType> DamageType;
	
	UPROPERTY(EditAnywhere, Category="Projectile")
	float DestroyTime = 5.f;
};
