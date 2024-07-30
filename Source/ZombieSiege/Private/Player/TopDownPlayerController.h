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
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* InPawn) override;
	
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

	UPROPERTY(EditAnywhere, Category="Look")
	float LookRaycastLimit = 3000.f;

	TWeakObjectPtr<APlayerCharacter> PlayerCharacter;
	
	FVector AimDirection = FVector(0.f);
	
	void Move(const FInputActionValue& Value);
	void FaceMouse();
	
	void Interact();
	
	void Fire();
	void StopFiring();
	void SwapWeapon();
	void ReloadWeapon();

	void OnMoneyChanged(const int32 NewMoneyAmount, const int32 AmountChanged);
};
