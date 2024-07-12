// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "ZombieDefenceGameMode.generated.h"

/**
 * 
 */
UCLASS()
class AZombieDefenceGameMode : public AGameMode
{
	GENERATED_BODY()

protected:
	virtual void OnPostLogin(AController* NewPlayer) override;

private:
	UPROPERTY(EditAnywhere, Category="Player", meta=(ClampMin=0, UIMin=0))
	int32 StartingMoney = 500;
};
