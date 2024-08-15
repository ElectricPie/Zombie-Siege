// Fill out your copyright notice in the Description page of Project Settings.


#include "Ui/Widgets/OptionsWidget.h"

#include "Components/Button.h"

void UOptionsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CloseButton->OnClicked.AddDynamic(this, &UOptionsWidget::OnCloseButtonClicked);
}

void UOptionsWidget::OnCloseButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Options Close"));
	OnOptionsClosedEvent.Broadcast();
}
