// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Barricade.generated.h"

class UMoneyRewardComponent;
class APlayerCharacter;
class UInteractableComponent;
class UArrowComponent;
class UBoxComponent;
UCLASS()
class ABarricade : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABarricade();

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsDestroyed() const { return DestroyedPlanks >= Planks.Num();  }
	UFUNCTION(BlueprintCallable)
	void Repair();
	
protected:
	UPROPERTY()
	USceneComponent* BaseComponent;
	UPROPERTY(VisibleAnywhere, Category="Components")
	UStaticMeshComponent* Mesh;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Components")
	UInteractableComponent* PlayerInteractionTrigger;
	UPROPERTY(VisibleAnywhere, Category="Components")
	UArrowComponent* InsideDirection;
	UPROPERTY(VisibleAnywhere, Category="Components")
	UMoneyRewardComponent* MoneyRewardComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Health")
	TArray<UStaticMeshComponent*> Planks;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Health")
	int32 DestroyedPlanks = 0;

	UPROPERTY(EditAnywhere, Category="Health", meta=(ClampMin=1, UIMin=1))
	int32 MaxHealth = 200;
	UPROPERTY(VisibleAnywhere, Category="Health")
	int32 CurrentHealth = 200;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnInteract(APlayerCharacter* InteractingPlayer);
};
