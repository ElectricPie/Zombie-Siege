// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBuyPoint.generated.h"

class AGun;
class UInteractableComponent;

UCLASS()
class AWeaponBuyPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBuyPoint();
	
private:
	void BuyWeapon(const AController* InteractionInstigator, const AActor* InteractionCauser);
	
private:
	UPROPERTY(VisibleInstanceOnly, Category="Components")
	TObjectPtr<UInteractableComponent> InteractableComponent;
	UPROPERTY(VisibleInstanceOnly, Category="Components")
	TObjectPtr<USkeletalMeshComponent> WeaponMeshComponent;

	UPROPERTY(EditAnywhere, Category="Weapon Buy Point")
	TSubclassOf<AGun> WeaponClass;
	UPROPERTY(EditAnywhere, Category="Weapon Buy Point", meta=(ClampMin=0, UIMin=0))
	int32 Cost = 100;

};
