// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TopDownPlayerController.generated.h"

class APlayerCharacter;
struct FInputActionValue;
class UInputMappingContext;
class UInputAction;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnMoneyChangedSinganture, int32 /*NewMoneyAmount*/, int32 /*AmountChanged*/);

/**
 * 
 */
UCLASS()
class ZOMBIESIEGE_API ATopDownPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	FOnMoneyChangedSinganture OnMoneyChangedEvent;
	
	FVector GetAimDirection() const { return AimDirection; }
	
	/**
	 * @brief Sets the players money to the provided amount
	 * @param AmountToSetTo The amount of money to set the player
	 */
	void SetMoney(const int32 AmountToSetTo);
	/**
	 * @brief Adds the given amount to the players current money
	 * @param AmountToAdd The amount of money to add
	 */
	void AddMoney(const int32 AmountToAdd);
	int32 GetMoney() const { return Money; }
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* InPawn) override;
	
private:
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

	UPROPERTY(EditAnywhere, Category="Look")
	float LookRaycastLimit = 3000.f;

	UPROPERTY(VisibleAnywhere, Category="Money")
	int32 Money;

	TWeakObjectPtr<APlayerCharacter> PlayerCharacter;
	
	FVector AimDirection = FVector(0.f);
	
	void Move(const FInputActionValue& Value);
	void FaceMouse();
	
	void Interact();
	
	void Fire();
	void StopFiring();
	void SwapWeapon();
	void ReloadWeapon();
};
