// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UnitSpawnPoint.generated.h"

class UCapsuleComponent;
class UArrowComponent;
class AUnitCharacter;
class ABarricade;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnSpawnPointActiveChangedSignature, AUnitSpawnPoint* /*BarricadeChanging*/, bool /*bNewActiveState*/);

UCLASS()
class AUnitSpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUnitSpawnPoint();

	bool GetIsForceDeactivated() const { return bForceDeactivate; }
	bool GetIsActive() const { return bIsActive; }
	
	/**
	 * @brief Attempts to spawn the given unit at this spawn point
	 * @param UnitClass The class of unit to spawn
	 * @return A pointer to the spawned unit or a nullptr if it fails to spawn
	 */
	TWeakObjectPtr<AUnitCharacter> SpawnUnit(TSubclassOf<AUnitCharacter> UnitClass);
	
public:
	FOnSpawnPointActiveChangedSignature OnActiveStateChangedEvent;
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<UCapsuleComponent> CapsuleComponent;
#if WITH_EDITORONLY_DATA
	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<UArrowComponent> SpawnDirectionArrow;
#endif
	
	UPROPERTY(EditAnywhere, Category="Barricades",meta=(ToolTip="The barricades that units spawned here will go to"))
	TArray<TWeakObjectPtr<ABarricade>> ConnectedBarricades;
	UPROPERTY(VisibleAnywhere, Category="Barricades")
	TArray<TWeakObjectPtr<ABarricade>> ActiveBarricades;

	UPROPERTY(EditAnywhere)
	bool bForceDeactivate = false;
	UPROPERTY(VisibleAnywhere)
	bool bIsActive = false;

	void OnBarricadeActiveChanged(ABarricade* BarricadeChanging, bool bNewActiveState);

private:
	TMap<TWeakObjectPtr<ABarricade>, FDelegateHandle> BarricadeChangedHandles;
};
