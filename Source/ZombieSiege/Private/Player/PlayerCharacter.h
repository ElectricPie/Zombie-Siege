// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class UHealthComponent;
class UCameraComponent;
class USpringArmComponent;
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
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	void Move(const FVector Direction);
	void LookAt(const FVector Pos);
	
protected:
	UPROPERTY(VisibleAnywhere, Category="Components")
	USpringArmComponent* CameraArm;
	UPROPERTY(VisibleAnywhere, Category="Components")
	UCameraComponent* Camera;
	UPROPERTY(VisibleAnywhere, Category="Components")
	UHealthComponent* Health;

	float SpeedModifier = 0.8f;
};
