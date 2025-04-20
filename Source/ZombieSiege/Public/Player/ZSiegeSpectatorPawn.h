// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpectatorPawn.h"
#include "ZSiegeSpectatorPawn.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class ZOMBIESIEGE_API AZSiegeSpectatorPawn : public ASpectatorPawn
{
	GENERATED_BODY()

public:
	AZSiegeSpectatorPawn();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void Tick(float DeltaSeconds) override;

	void SetFollowTarget(AActor* NewTarget);
	
private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> CameraArm;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(VisibleAnywhere, Replicated)
	TWeakObjectPtr<AActor> FollowTarget;
};
