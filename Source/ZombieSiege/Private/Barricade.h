// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Barricade.generated.h"

class UArrowComponent;
class UBoxComponent;
UCLASS()
class ABarricade : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABarricade();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector GetOutsideTriggerPos() const;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsDestroyed() const { return CurrentHealth <= 0; };
	
protected:
	UPROPERTY()
	USceneComponent* BaseComponent;
	UPROPERTY(VisibleAnywhere, Category="Components")
	UStaticMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere, Category="Components")
	UBoxComponent* OuterTrigger;
	UPROPERTY(VisibleAnywhere, Category="Components")
	UBoxComponent* InsideTrigger;
	UPROPERTY(VisibleAnywhere, Category="Components")
	UArrowComponent* InsideDirection;

	UPROPERTY(EditAnywhere, Category="Health", meta=(ClampMin=1, UIMin=1))
	int32 MaxHealth = 200;
	UPROPERTY(VisibleAnywhere, Category="Health")
	int32 CurrentHealth = 200;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
};
