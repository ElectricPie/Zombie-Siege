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

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Barricade")
	bool IsDestroyed() const { return DestroyedPlanks >= Planks.Num();  }
	UFUNCTION(BlueprintCallable, Category="Barricade")
	void Repair();
	UFUNCTION(BlueprintCallable, Category="Barricade")
	void SetIsActive(const bool bNewIsActive) { bIsActive = bNewIsActive; }
	UFUNCTION(BlueprintPure, Category="Barricade")
	bool GetIsActive() const { return bIsActive; }
	
protected:
	UPROPERTY()
	USceneComponent* BaseComponent;
	UPROPERTY(VisibleAnywhere, Category="Components")
	UStaticMeshComponent* Mesh;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Components")
	TObjectPtr<UInteractableComponent> PlayerInteractionTrigger;
	UPROPERTY(VisibleAnywhere, Category="Components")
	UArrowComponent* InsideDirection;
	UPROPERTY(VisibleAnywhere, Category="Components")
	UMoneyRewardComponent* MoneyRewardComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Health")
	TArray<UStaticMeshComponent*> Planks;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Health")
	int32 DestroyedPlanks = 0;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	bool bIsActive = false;
	
	UFUNCTION()
	void OnInteract(TWeakObjectPtr<AController> InteractionInstigator, TWeakObjectPtr<AActor> InteractionCauser);
};
