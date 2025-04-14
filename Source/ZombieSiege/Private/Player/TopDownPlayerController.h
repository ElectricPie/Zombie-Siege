// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ZombieSiege/Public/Money/MoneyStoreInterface.h"
#include "TopDownPlayerController.generated.h"

class UMoneyStoreComponent;
class APlayerCharacter;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

/**
 * 
 */
UCLASS()
class ZOMBIESIEGE_API ATopDownPlayerController : public APlayerController, public IMoneyStoreInterface
{
	GENERATED_BODY()

public:
	FVector GetAimDirection() const { return AimDirection; }
	void GameOver();
	void SetInputGameOnly();
	void SetInputGameAndUI();

	/* MoneyStoreInterface */
	virtual UMoneyStoreComponent* GetMoneyStoreComponent_Implementation() const override;
	/* End MoneyStoreInterface */

public:
	bool bIsPaused = false;
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnRep_PlayerState() override;
	
private:
	void Move(const FInputActionValue& Value);
	void FaceMouse();
	
	void Interact();
	
	void Fire();
	void StopFiring();
	void SwapWeapon();
	void ReloadWeapon();

	/**
	 * @brief Check if the player can do any action, this includes checking if the game is over or if the player
	 * character is valid and dead
	 * @return True if the player can do any action, false otherwise
	 */
	bool CanDoAction() const;

	void ToggleMenu();
	void OnPauseMenuChanged(const bool bMenuIsOpen);
	
private:
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputMappingContext> InputMappingContext;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> InteractAction;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> FireAction;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> SwapWeaponAction;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> ReloadWeaponAction;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> MenuAction;

	UPROPERTY(EditAnywhere, Category="Look")
	float LookRaycastLimit = 3000.f;
	
	FVector AimDirection = FVector(0.f);

	bool bIsGameOver = false;
};
