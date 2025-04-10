// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ZombieSiege/Public/Money/MoneyStoreInterface.h"
#include "DefencePlayerState.generated.h"

class UMoneyStoreComponent;

/**
 * 
 */
UCLASS(Abstract)
class ADefencePlayerState : public APlayerState, public IMoneyStoreInterface
{
	GENERATED_BODY()

public:
	ADefencePlayerState();
	
	void AddDeath() { TotalDeaths++; }
	void AddKill() { TotalKills++; }

	UFUNCTION(BlueprintGetter, Category="Player")
	int32 GetTotalKills() const { return TotalKills; }
	UFUNCTION(BlueprintGetter, Category="Player")
	int32 GetTotalDeaths() const { return TotalDeaths; }

	/* MoneyStoreInterface */
	virtual UMoneyStoreComponent* GetMoneyStoreComponent_Implementation() const override;
	/* End of MoneyStoreInterface */

protected:
	virtual void BeginPlay() override;

private:
	TObjectPtr<UMoneyStoreComponent> MoneyStoreComponent;
	
	UPROPERTY(VisibleAnywhere, Category="Player", BlueprintGetter=GetTotalKills)
	int32 TotalKills = 0;
	UPROPERTY(VisibleAnywhere, Category="Player", BlueprintGetter=GetTotalDeaths)
	int32 TotalDeaths = 0;
};
