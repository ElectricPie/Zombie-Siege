// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/MoneyRewardComponent.h"

#include "MoneyStoreComponent.h"

// Sets default values for this component's properties
UMoneyRewardComponent::UMoneyRewardComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

void UMoneyRewardComponent::RewardMoney(const AActor* ActorToReward) const
{
	UE_LOG(LogTemp, Warning, TEXT("Rewarding"));
	if (ActorToReward == nullptr) return;

	if (UMoneyStoreComponent* MoneyStore = ActorToReward->GetComponentByClass<UMoneyStoreComponent>())
	{
		MoneyStore->AddMoney(AmountToGive);	
	}
}

