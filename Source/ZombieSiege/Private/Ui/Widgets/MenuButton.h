// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuButton.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMenuButtonClickedSignature);

class UButton;
class UTextBlock;

/**
 * 
 */
UCLASS()
class UMenuButton : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativePreConstruct() override;

	virtual void NativeConstruct() override;
	
public:
	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnMenuButtonClickedSignature OnClicked;

private:
	UFUNCTION()
	void OnButtonClicked();
	
private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button = nullptr;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> TextBlock = nullptr;

	UPROPERTY(EditAnywhere, Category="MenuButton")
	FText Text = FText::FromString("Menu Button");
};
