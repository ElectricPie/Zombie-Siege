// Fill out your copyright notice in the Description page of Project Settings.


#include "Ui/Widgets/HealthWidget.h"
#include "Components/HealthComponent.h"
#include "Components/ProgressBar.h"

void UHealthWidget::Setup(UHealthComponent* NewHealthComponent, const int32 NewSegments)
{
	if (NewHealthComponent == nullptr)
		return;

	if (HealthComponent.IsValid())
	{
		HealthComponent->OnTakeDamageEvent.RemoveDynamic(this, &UHealthWidget::OnHealthChanged);
	}

	HealthComponent = NewHealthComponent;

	Segments = NewSegments;

	// Update timer
	if (TimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}

	HealthComponent->OnTakeDamageEvent.AddDynamic(this, &UHealthWidget::OnHealthChanged);
	
	UpdateHealthBar();

	UE_LOG(LogTemp, Warning, TEXT("UpdateTime: %f"), HealthBarUpdateTime);
	const FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &UHealthWidget::UpdateHealthBar);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, HealthBarUpdateTime, true);
}

void UHealthWidget::UpdateHealthBar()
{
	if (!HealthComponent.IsValid())
		return;
	
	const float HealthPercentage = HealthComponent->GetCurrentHealth() / HealthComponent->GetMaxHealth();
	HealthBar->SetPercent(HealthPercentage);
}

void UHealthWidget::OnHealthChanged(UHealthComponent* ChangedHealthComponent, float DamageAmount)
{
	UpdateHealthBar();
}
