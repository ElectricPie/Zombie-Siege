// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/MoneyRewardComponent.h"

#include "MoneyStoreComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UMoneyRewardComponent::UMoneyRewardComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

}

int32 UMoneyRewardComponent::RewardMoney(const AActor* ActorToReward)
{
	if (ActorToReward == nullptr) return 0;
	if (GetWorld() == nullptr) return 0;

	const double CurrentTime = UGameplayStatics::GetTimeSeconds(GetWorld());
	if (TimeBetweenRewards != 0.f)
	{
		if (CurrentTime - LastRewardAt < TimeBetweenRewards)
		{
			return 0;
		}
	}

	if (UMoneyStoreComponent* MoneyStore = ActorToReward->GetComponentByClass<UMoneyStoreComponent>())
	{
		MoneyStore->AddMoney(AmountToGive);	
		LastRewardAt = CurrentTime;
		
		return AmountToGive;
	}

	return 0;
}

void UMoneyRewardComponent::BeginPlay()
{
	Super::BeginPlay();

	LastRewardAt = -TimeBetweenRewards;
}
