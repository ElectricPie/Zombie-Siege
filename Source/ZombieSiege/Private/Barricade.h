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
	FVector GetOutsideTriggerPos();

protected:
	UPROPERTY(VisibleAnywhere, Category="Components")
	UStaticMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere, Category="Components")
	UBoxComponent* OuterTrigger;
	UPROPERTY(VisibleAnywhere, Category="Components")
	UBoxComponent* InsideTrigger;
	UPROPERTY(VisibleAnywhere, Category="Components")
	UArrowComponent* InsideDirection;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
};
