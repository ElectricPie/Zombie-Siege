// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GunProjectile.generated.h"

class ATopDownPlayerController;
class USphereComponent;
class UProjectileMovementComponent;

UCLASS(Abstract)
class AGunProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGunProjectile();
	
	UPROPERTY(BlueprintReadWrite, Category=Damage)
	float Damage;
	UPROPERTY(BlueprintReadWrite, Category=Projectile)
	ATopDownPlayerController* Shooter;
	UPROPERTY(BlueprintReadWrite, Category=Projectile)
	TSubclassOf<UDamageType> DamageType;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, Category=Components)
	USphereComponent* CollisionComponent;
	UPROPERTY(VisibleAnywhere, Category=Components)
	UStaticMeshComponent* ProjectileMesh;
	UPROPERTY(VisibleAnywhere, Category=Components)
	UProjectileMovementComponent* ProjectileMovementComponent;
	
	UPROPERTY(EditAnywhere, Category=Projectile)
	float DestroyTime = 5.f;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void DestroyProjectile();
};
