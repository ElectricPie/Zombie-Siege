// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/MoneyStoreComponent.h"

#include "FMODBlueprintStatics.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UMoneyStoreComponent::UMoneyStoreComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UMoneyStoreComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UMoneyStoreComponent, Money);
}

void UMoneyStoreComponent::SetMoney_Server(const int32 AmountToSetTo)
{
	check(GetOwner()->HasAuthority());
	
	Money = AmountToSetTo;
	OnMoneyChangedEvent.Broadcast(Money, 0);
}

void UMoneyStoreComponent::AddMoney(const int32 AmountToAdd)
{
	Money = FMath::Max(0, Money + AmountToAdd);
	
	OnMoneyChangedEvent.Broadcast(Money, AmountToAdd);
	if (MoneyGetSound)
	{
		UFMODBlueprintStatics::PlayEvent2D(GetWorld(), MoneyGetSound, true);
	}
}

bool UMoneyStoreComponent::TakeMoney(const int32 AmountToTake)
{
	// Invalid amount
	if (AmountToTake < 0)
		return false;
	
	if (Money >= AmountToTake)
	{
		Money -= AmountToTake;
		OnMoneyChangedEvent.Broadcast(Money, -AmountToTake);

		if (MoneySpendSound)
		{
			UFMODBlueprintStatics::PlayEvent2D(GetWorld(), MoneySpendSound, true);
		}
		
		return true;
	}

	return false;
}

void UMoneyStoreComponent::OnRep_Money() const
{
	OnMoneyChangedEvent.Broadcast(Money, 0);
}

