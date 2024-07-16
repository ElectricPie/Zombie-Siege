// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UnitSpawnPoint.generated.h"

class UCapsuleComponent;
class UArrowComponent;
class ABarricade;
UCLASS()
class AUnitSpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUnitSpawnPoint();

	bool GetIsActive() const { return bIsActive; }
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<UCapsuleComponent> CapsuleComponent;
#if WITH_EDITORONLY_DATA
	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<UArrowComponent> SpawnDirectionArrow;
#endif
	
	UPROPERTY(EditAnywhere, meta=(ToolTip="The barricades that units spawned here will go to"))
	TArray<TWeakObjectPtr<ABarricade>> ConnectedBarricades;
	
	UPROPERTY(VisibleAnywhere)
	bool bIsActive = false;
