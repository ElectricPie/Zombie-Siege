// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "WidgetController.generated.h"

USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()

	FWidgetControllerParams() {}
	FWidgetControllerParams(APlayerController* InPlayerController) : PlayerController(InPlayerController) {}

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<APlayerController> PlayerController;
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
	
	virtual void BindCallbackToDependencies();
	virtual void BroadcastInitialValues();
	
protected:
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<APlayerController> PlayerController;

};
