// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "WidgetController.generated.h"

USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()

	FWidgetControllerParams()
	{
	}

	FWidgetControllerParams(APlayerController* InPlayerController, APlayerState* InPlayerState) :
		PlayerController(InPlayerController), PlayerState(InPlayerState)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<APlayerController> PlayerController;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<APlayerState> PlayerState;
};

/**
 * 
 */
UCLASS(Abstract, BlueprintType)
class ZOMBIESIEGE_API UWidgetController : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParams(const FWidgetControllerParams& InParams);

	/**
	 * Called when the widget is created. This is where you should bind any events this controller needs to listen to.
	 */
	virtual void BindCallbackToDependencies();
	/**
	 * Called to do an initial broadcast of values to any listeners.
	 */
	UFUNCTION(BlueprintCallable)
	virtual void BroadcastInitialValues();

protected:
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<APlayerController> PlayerController;
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<APlayerState> PlayerState;
};
