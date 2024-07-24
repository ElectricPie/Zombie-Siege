// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBuyPoint.generated.h"

class UInteractableComponent;
UCLASS()
class AWeaponBuyPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBuyPoint();

private:
	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<UInteractableComponent> InteractableComponent;

};
