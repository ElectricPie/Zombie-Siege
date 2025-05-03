// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Health/HealthComponentInterface.h"
#include "ZombieSiege/Public/Money/MoneyStoreInterface.h"
#include "PlayerCharacter.generated.h"

class UPlayerHealthComponent;
class AGunBase;
class UAnimMontage;
class UCameraComponent;
class UInteractableComponent;
class UInteractorComponent;
class USpringArmComponent;
class UWeaponLoadoutComponent;

UCLASS()
class APlayerCharacter : public ACharacter, public IMoneyStoreInterface, public IHealthComponentInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	/**
	 * @brief Gets the velocity relative to the direction they are facing
	 * @return Returns positive values if moving forward and negative for backwards
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsMovingForward() const;

	void Move(const FVector Direction);
	void Interact() const;

	UFUNCTION(BlueprintSetter)
	UWeaponLoadoutComponent* GetWeaponLoadoutComponent() const { return WeaponLoadoutComponent; }

	void Fire() const;
	void StopFiring() const;
	void ReloadWeapon() const;

	/* MoneyStoreInterface */
	virtual UMoneyStoreComponent* GetMoneyStoreComponent_Implementation() const override;
	/* End MoneyStoreInterface */
	/* HealthComponentInterface */
	virtual UHealthComponent* GetHealthComponent_Implementation() const override;
	/* End HealthComponentInterface */

	UFUNCTION(BlueprintPure)
	UInteractorComponent* GetInteractorComponent() const { return InteractorComponent; }

	void SetAimLocation(const FVector& NewAimLocation) const;

	void SetDesiredHeadGearMesh_Server(UStaticMesh* NewMesh);

protected:
	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<USpringArmComponent> CameraArm;
	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<UCameraComponent> Camera;
	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<UInteractorComponent> InteractorComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	TObjectPtr<UWeaponLoadoutComponent> WeaponLoadoutComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UPlayerHealthComponent> HealthComponent;
	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<UStaticMeshComponent> HeadGearMesh;

	float SpeedModifier = 0.8f;

protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditAnywhere, Category="Weapon",
		meta=(ToolTip="The time a reload takes if the equiped gun has no reload animation"))
	float DefaultReloadTime = 2.f;
	bool bIsReloading = false;
	FTimerHandle ReloadingTimerHandle;

	UPROPERTY(EditAnywhere, Category="Movement",
		meta=(ToolTip="How far from forward the character can move before they are considered to be moving backwards",
			ClampMin="-1.0", ClampMax="1.0", UIMin="-1.0", UIMax="1.0"))
	float BackwardsThreshold = -0.5f;

	UPROPERTY(EditAnywhere, Category="PlayerCharacter|Animation")
	FName PistolWeaponSocket = TEXT("PistolSocket");
	UPROPERTY(EditAnywhere, Category="PlayerCharacter|Animation")
	FName RifleWeaponSocket = TEXT("RifleSocket");

	UPROPERTY(ReplicatedUsing=OnRep_DesiredHeadGearMesh)
	TObjectPtr<UStaticMesh> DesiredHeadGearMesh;

private:
	void OnWeaponAdded(AGunBase* Weapon);
	UFUNCTION()
	void Die_Server(AActor* VictimActor, AController* KillerController, AActor* KillerActor);
	
	UFUNCTION()
	void OnOverlap(AActor* OverlappedActor, AActor* OtherActor);
	UFUNCTION()
	void OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void OnRep_DesiredHeadGearMesh() const;
};
