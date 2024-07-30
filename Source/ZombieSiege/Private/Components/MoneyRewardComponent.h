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

	
	/**
	 * @brief Gives the MoneyStoreComponent attached to the ActorToReward the AmountToGive
	 * @param ActorToReward The actor to give the money to
	 * @return The amount of money given to the store, 0 if the store is not found or the time between rewards has not passed
	 */
	int32 RewardMoney(const AActor* ActorToReward);
	void SetAmountToGive(const int32 NewAmountToGive) { AmountToGive = NewAmountToGive; }
	void SetTimeBetweenRewards(const float NewTimeBetweenRewards) { TimeBetweenRewards = NewTimeBetweenRewards; }

protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditAnywhere, Category="Money", meta=(ToolTip="The amount of money to give the store on reward"))
	int32 AmountToGive = 100;
	UPROPERTY(EditAnywhere, Category="Money", meta=(ClampMin=0.f, UIMin=0.f, ToolTip="The time between rewards being given"))
	float TimeBetweenRewards = 0.f;

	UPROPERTY(VisibleAnywhere)
	double LastRewardAt;
};
