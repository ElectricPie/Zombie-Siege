// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MoneyStoreComponent.generated.h"

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
	
private:
	UPROPERTY(VisibleAnywhere, Category="Money")
	int32 Money = 0;
};
