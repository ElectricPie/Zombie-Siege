// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OptionsWidget.generated.h"

class UButton;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOptionsClosedSignature);

/**
 * 
 */
UCLASS()
class UOptionsWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnOptionsClosedSignature OnOptionsClosedEvent;

protected:
	virtual void NativeConstruct() override;
	
private:
	UFUNCTION()
	void OnCloseButtonClicked();
	
private:
	UPROPERTY(VisibleAnywhere, Category="Widgets", meta=(BindWidget))
	UButton* CloseButton; 
};
