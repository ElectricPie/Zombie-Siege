// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieSiege/Public/Ui/Widgets/ZSiegeUserWidget.h"

void UZSiegeUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}
