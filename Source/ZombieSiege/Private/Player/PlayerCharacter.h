// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class UFMODAudioComponent;
class AGun;
class UAnimMontage;
class UCameraComponent;
class UFMODEvent;
class UInteractableComponent;
class UInteractorComponent;
class USpringArmComponent;
class UWeaponLoadoutComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerDeath, APlayerCharacter*, PlayerCharacter);

UCLASS()
class APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	virtual void PostInitProperties() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	/**
	 * @brief Gets the velocity relative to the direction they are facing
	 * @return Returns positive values if moving forward and negative for backwards
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsMovingForward() const;
	
	void Move(const FVector Direction);
	void Interact();

	UFUNCTION(BlueprintSetter)
	UWeaponLoadoutComponent* GetWeaponLoadoutComponent() const { return WeaponLoadoutComponent; }
	void Fire(AController* Shooter);
	void StopFiring();
	void ReloadWeapon();

	UFUNCTION(BlueprintPure)
	bool GetIsDead() const { return bIsDead; }

public:
	UPROPERTY(BlueprintAssignable)
	FOnPlayerDeath OnPlayerDeathEvent;
	
protected:
	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<USpringArmComponent> CameraArm;
	UPROPERTY(VisibleAnywhere, Category="Components")
    TObjectPtr<UCameraComponent> Camera;
	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<UInteractorComponent> InteractorComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	TObjectPtr<UWeaponLoadoutComponent> WeaponLoadoutComponent;

	float SpeedModifier = 0.8f;

private:
	void OnInteractionEntered(TWeakObjectPtr<UInteractableComponent> InteractableComponent);
	void OnInteractionExited(TWeakObjectPtr<UInteractableComponent> InteractableComponent);

	void OnWeaponAdded(AGun* Weapon);
	void Die();
	
private:
	UPROPERTY(EditAnywhere, Category="Weapon", meta=(ToolTip="The time a reload takes if the equiped gun has no reload animation"))
	float DefaultReloadTime = 2.f;
	bool bIsReloading = false;
	FTimerHandle ReloadingTimerHandle;
	
	UPROPERTY(EditAnywhere, Category="Health", meta=(ClampMin=0.f, UIMin=0.f))
	float MaxHealth = 30.f;
	UPROPERTY(VisibleAnywhere, Category="Health")
	float CurrentHealth = 30.f;
	UPROPERTY(VisibleAnywhere, Category="Health")
	bool bIsDead = false;

	UPROPERTY(EditAnywhere, Category="Movement", meta=(ToolTip="How far from forward the character can move before they are considered to be moving backwards", ClampMin="-1.0", ClampMax="1.0", UIMin="-1.0", UIMax="1.0"))
	float BackwardsThreshold = -0.5f;

	UPROPERTY(EditAnywhere, Category="Animation")
	FName WeaponSocketName = TEXT("WeaponSocket");

	UPROPERTY(EditAnywhere, Category="Sound")
	UFMODEvent* HitSound;
	TWeakObjectPtr<UFMODAudioComponent> HitSoundComponent = nullptr;
};