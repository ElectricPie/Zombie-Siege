 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthWidget.generated.h"

class UHealthComponent;
class UProgressBar;

/**
 * 
 */
UCLASS()
class UHealthWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void Setup(UHealthComponent* NewHealthComponent, const int32 NewSegments);

private:
	void UpdateHealthBar();
	UFUNCTION()
	void OnHealthChanged(UHealthComponent* HealthComponent, float DamageAmount);
	
private:
	UPROPERTY(VisibleAnywhere, Category="Health", meta=(BindWidget))
	TObjectPtr<UProgressBar> HealthBar;
	UPROPERTY(EditInstanceOnly, Category="Health", meta=(ClampMin=0.f, UIMin=0.f, ToolTip="The rate at which the health bar updates"))
	float HealthBarUpdateTime = 1.f;

	TWeakObjectPtr<UHealthComponent> HealthComponent;

	int32 Segments = 3;
	
	FTimerHandle TimerHandle;
};
