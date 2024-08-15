// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TopDownPlayerController.generated.h"

class UMoneyStoreComponent;
class APlayerCharacter;
struct FInputActionValue;
class UInputMappingContext;
class UInputAction;

/**
 * 
 */
UCLASS()
class ZOMBIESIEGE_API ATopDownPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATopDownPlayerController();
	
	FVector GetAimDirection() const { return AimDirection; }
	void GameOver();

public:
	bool bIsPaused = false;
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* InPawn) override;
	
private:
	void Move(const FInputActionValue& Value);
	void FaceMouse();
	
	void Interact();
	
	void Fire();
	void StopFiring();
	void SwapWeapon();
	void ReloadWeapon();

	void OnMoneyChanged(const int32 NewMoneyAmount, const int32 AmountChanged);

	/**
	 * @brief Check if the player can do any action, this includes checking if the game is over or if the player
	 * character is valid and dead
	 * @return True if the player can do any action, false otherwise
	 */
	bool CanDoAction() const;

	void ToggleMenu();
	void OnPauseMenuChanged(const bool bMenuIsOpen);
	
private:
	UPROPERTY(VisibleAnywhere, Category="Money")
	UMoneyStoreComponent* MoneyStoreComponent;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputMappingContext* InputMappingContext;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* MoveAction;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* InteractAction;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* FireAction;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* SwapWeaponAction;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* ReloadWeaponAction;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* MenuAction;

	UPROPERTY(EditAnywhere, Category="Look")
	float LookRaycastLimit = 3000.f;

	TWeakObjectPtr<APlayerCharacter> PlayerCharacter;
	
	FVector AimDirection = FVector(0.f);

	bool bIsGameOver = false;
};
