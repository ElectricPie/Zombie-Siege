// Fill out your copyright notice in the Description page of Project Settings.


#include "Health/PlayerHealthComponent.h"

#include "FMODAudioComponent.h"
#include "FMODBlueprintStatics.h"

void UPlayerHealthComponent::TickComponent(const float DeltaTime, const ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (HealthSoundComponent)
	{
		const float HealthPercentage = GetHealthPercentage();
		HealthSoundComponent->SetParameter(HealthSoundParameterName, HealthPercentage * 100.f);
	}
}

void UPlayerHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// Only play sound if the player is local
	if (!GetOwner<APawn>()->IsLocallyControlled())
		return;
	
	if (HealthSound)
	{
		HealthSoundComponent = UFMODBlueprintStatics::PlayEventAttached(
			HealthSound,
			GetOwner()->GetRootComponent(),
			NAME_None,
			FVector::ZeroVector,
			EAttachLocation::KeepRelativeOffset,
			true,
			true,
			true);
	}
}
