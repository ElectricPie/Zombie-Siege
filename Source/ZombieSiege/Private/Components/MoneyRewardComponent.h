// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MoneyRewardComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UMoneyRewardComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMoneyRewardComponent();

	void RewardMoney(const AActor* ActorToReward) const;
	
private:
	UPROPERTY(EditAnywhere, Category="Money", meta=(ToolTip="The amount of money to give the store on reward"))
	int32 AmountToGive = 100;
};
