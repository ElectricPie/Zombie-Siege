// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Barricade.generated.h"


class UFMODEvent;
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnActiveChangedSignature, ABarricade* /*BarricadeChanging*/, bool /*bNewActiveState*/);

struct FNavigationLink;
class UNavLinkComponent;
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
	bool IsDestroyed() const { return DestroyedPlanks >= Planks.Num(); }

	UFUNCTION(BlueprintCallable, Category="Barricade")
	void SetIsActive(const bool bNewIsActive);
	UFUNCTION(BlueprintPure, Category="Barricade")
	bool GetIsActive() const { return bIsActive; }

	UFUNCTION(BlueprintPure, Category="Barricade")
	FVector GetInsideLocation() const;
	UFUNCTION(BlueprintPure, Category="Barricade")
	FVector GetOutsideLocation() const;
	
	UFUNCTION(BlueprintCallable, Category="Barricade")
	void StartCrossing(AActor* Agent);
	UFUNCTION(BlueprintCallable, Category="Barricade")
	void StopCrossing(AActor* Agent);
	
public:
	FOnActiveChangedSignature OnActiveChangedEvent;

protected:
	UPROPERTY()
	TObjectPtr<USceneComponent> BaseComponent;
	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<UStaticMeshComponent> Mesh;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Components")
	TObjectPtr<UInteractableComponent> PlayerInteractionTrigger;
	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<UArrowComponent> InsideDirection;
	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<UMoneyRewardComponent> MoneyRewardComponent;
	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<UNavLinkComponent> NavLinkComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Health")
	TArray<UStaticMeshComponent*> Planks;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Health")
	int32 DestroyedPlanks = 0;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	bool bIsActive = false;

	TArray<TWeakObjectPtr<AActor>> AgentsCrossing;

	UPROPERTY(EditAnywhere, Category="Audio")
	TObjectPtr<UFMODEvent> HitSound;
	UPROPERTY(EditAnywhere, Category="Audio")
	TObjectPtr<UFMODEvent> DestructionSound;

	UPROPERTY(EditAnywhere, Category="Effects")
	TObjectPtr<UParticleSystem> HitEffect;
	
private:
	UFUNCTION()
	void OnInteract(AController* InteractionInstigator, APawn* InteractionCauser);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastDestroyPlank(UStaticMeshComponent* Plank);
	
	UFUNCTION(BlueprintCallable, Category="Barricade")
	void Repair_Server();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRepair();

};
