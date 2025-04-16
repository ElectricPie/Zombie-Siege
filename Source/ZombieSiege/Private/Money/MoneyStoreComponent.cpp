// Fill out your copyright notice in the Description page of Project Settings.


#include "Money/MoneyStoreComponent.h"

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

void UMoneyStoreComponent::AddMoney_Server(const int32 AmountToAdd)
{
	check(GetOwner()->HasAuthority());

	Money = FMath::Max(0, Money + AmountToAdd);

	if (MoneyGetSound)
	{
		UFMODBlueprintStatics::PlayEvent2D(GetWorld(), MoneyGetSound, true);
	}
	OnMoneyChangedEvent.Broadcast(Money, AmountToAdd);
	ClientMoneyChanged(Money, AmountToAdd);
}

bool UMoneyStoreComponent::TakeMoney_Server(const int32 AmountToTake)
{
	check(GetOwner()->HasAuthority());

	// Invalid amount
	if (AmountToTake < 0)
		return false;

	if (Money >= AmountToTake)
	{
		Money -= AmountToTake;

		if (MoneySpendSound)
		{
			UFMODBlueprintStatics::PlayEvent2D(GetWorld(), MoneySpendSound, true);
		}

		OnMoneyChangedEvent.Broadcast(Money, -AmountToTake);
		ClientMoneyChanged(Money, -AmountToTake);

		return true;
	}

	return false;
}

void UMoneyStoreComponent::ClientMoneyChanged_Implementation(const int32 NewMoney, const int32 AmountChanged)
{
	if (AmountChanged > 0)
	{
		if (MoneyGetSound)
		{
			UFMODBlueprintStatics::PlayEvent2D(GetWorld(), MoneyGetSound, true);
		}
	}
	else if (AmountChanged < 0)
	{
		if (MoneySpendSound)
		{
			UFMODBlueprintStatics::PlayEvent2D(GetWorld(), MoneySpendSound, true);
		}
	}

	OnMoneyChangedEvent.Broadcast(NewMoney, AmountChanged);
}
