// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class UInteractableComponent;
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

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
	
public:
	void Move(const FVector Direction);
	void LookAt(const FVector Pos);
	void Interact();

	void AddInteractable(UInteractableComponent* InteractableComponent);
	void RemoveInteractable(const UInteractableComponent* InteractableComponent);
	
protected:
	UPROPERTY(VisibleAnywhere, Category="Components")
	USpringArmComponent* CameraArm;
	UPROPERTY(VisibleAnywhere, Category="Components")
	UCameraComponent* Camera;

	float SpeedModifier = 0.8f;
	
private:
	UPROPERTY(EditAnywhere, Category="Health", meta=(ClampMin=0.f, UIMin=0.f))
	float MaxHealth = 100.f;
	float CurrentHealth;

	TSet<UInteractableComponent*> NearbyIntractables;
};
