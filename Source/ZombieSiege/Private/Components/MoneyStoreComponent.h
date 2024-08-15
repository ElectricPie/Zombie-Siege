// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MoneyStoreComponent.generated.h"

class UFMODEvent;
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnMoneyChangedSinganture, int32 /*NewMoneyAmount*/, int32 /*AmountChanged*/);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UMoneyStoreComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMoneyStoreComponent();
	
	FOnMoneyChangedSinganture OnMoneyChangedEvent;
	
	/**
	 * @brief Sets the players money to the provided amount
	 * @param AmountToSetTo The amount of money to set the player
	 */
	UFUNCTION(BlueprintCallable, Category="Money")
	void SetMoney(const int32 AmountToSetTo);
	/**
	 * @brief Adds the given amount to the players current money
	 * @param AmountToAdd The amount of money to add
	 */
	UFUNCTION(BlueprintCallable, Category="Money")
	void AddMoney(const int32 AmountToAdd);
	UFUNCTION(BlueprintPure, Category="Money")
	int32 GetMoney() const { return Money; }
	/**
	 * @brief Attempts to take the amount provided from the store, if there is the money will be taken otherwise nothing
	 * will happen
	 * @param AmountToTake How much money to take
	 * @return Returns true if the money can be taken, false otherwise
	 */
	UFUNCTION(BlueprintCallable, Category="Money")
	bool TakeMoney(int32 AmountToTake);
	
private:
	UPROPERTY(VisibleAnywhere, Category="Money")
	int32 Money = 0;

	UPROPERTY(EditAnywhere, Category="Sound")
	UFMODEvent* MoneyGetSound;
	UPROPERTY(EditAnywhere, Category="Sound")
	UFMODEvent* MoneySpendSound;
};
