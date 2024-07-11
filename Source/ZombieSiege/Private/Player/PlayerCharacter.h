// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class AGun;
class ATopDownPlayerController;
class UInteractableComponent;
class UHealthComponent;
class UCameraComponent;
class USpringArmComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponChangedSignature, AGun*, NewWeapon);

UCLASS()
class APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintAssignable, Category=Weapon)
	FOnWeaponChangedSignature OnWeaponChangedEvent;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	AGun* GetEquippedWeapon();
	
	/**
	 * @brief Gets the velocity relative to the direction they are facing
	 * @return Returns positive values if moving forward and negative for backwards
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsMovingForward() const;
	
	void Move(const FVector Direction);
	void Interact();
	
	void Fire(ATopDownPlayerController* Shooter);
	void StopFiring();
	void NextWeapon();
	void ReloadWeapon();

	void AddInteractable(UInteractableComponent* InteractableComponent);
	void RemoveInteractable(const UInteractableComponent* InteractableComponent);

	
protected:
	UPROPERTY(VisibleAnywhere, Category=Components)
	USpringArmComponent* CameraArm;
	UPROPERTY(VisibleAnywhere, Category=Components)
	UCameraComponent* Camera;

	float SpeedModifier = 0.8f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Weapon, meta=(ClampMin=0, UIMin=0))
	int32 EquippedWeaponIndex;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Weapon)
	TArray<AGun*> Weapons;
	
private:
	UPROPERTY(EditAnywhere, Category=Health, meta=(ClampMin=0.f, UIMin=0.f))
	float MaxHealth = 100.f;
	float CurrentHealth;

	UPROPERTY(EditAnywhere, Category=Movement, meta=(ToolTip="How far from forward the character can move before they are considered to be moving backwards", ClampMin="-1.0", ClampMax="1.0", UIMin="-1.0", UIMax="1.0"))
	float BackwardsThreshold = -0.5f;

	TSet<UInteractableComponent*> NearbyIntractables;
};
