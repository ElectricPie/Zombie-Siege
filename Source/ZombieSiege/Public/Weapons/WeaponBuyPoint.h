// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBuyPoint.generated.h"

class UMoneyStoreComponent;
class UWeaponBuyPointDataAsset;
class UInteractableComponent;

UCLASS()
class AWeaponBuyPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBuyPoint();
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<UInteractableComponent> InteractableComponent;
	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<USkeletalMeshComponent> WeaponMeshComponent;

	UPROPERTY(EditAnywhere, Category="Weapon")
	TObjectPtr<UWeaponBuyPointDataAsset> WeaponBuyPointDataAsset;

private:
	UFUNCTION(CallInEditor, Category="Weapon", meta=(DisplayName="ForceMeshRefresh", ToolTip="Forces the weapon mesh to update to match the one in the data asset, this is only needed after changing the mesh in the data asset without restarting the editor"))
	void RefreshWeaponMesh() const;
	
	bool TryBuyWeapon_Server(UMoneyStoreComponent* MoneyStore, APawn* ActorToGiveWeapon) ;
};
