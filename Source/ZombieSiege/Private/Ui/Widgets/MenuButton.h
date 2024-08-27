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
	UPROPERTY(VisibleAnywhere, Category="Widgets", meta=(BindWidget))
	TObjectPtr<UButton> Button = nullptr;
	UPROPERTY(VisibleAnywhere, Category="Widgets", meta=(BindWidget))
	TObjectPtr<UTextBlock> Text = nullptr;

	UPROPERTY(EditAnywhere, Category="MenuButton")
	FText ButtonText = FText::FromString("Menu Button");
	UPROPERTY(EditAnywhere, Category="MenuButton")
	FLinearColor ButtonColor = FLinearColor::White;
};
