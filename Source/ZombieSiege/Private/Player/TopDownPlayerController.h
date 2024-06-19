// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TopDownPlayerController.generated.h"

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

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupInputComponent() override;
	
private:
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputMappingContext* InputMappingContext;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category="Look")
	float LookRaycastLimit = 3000.f;
	
	
	
	void Move(const FInputActionValue& Value);
	void FaceMouse();
	
};
