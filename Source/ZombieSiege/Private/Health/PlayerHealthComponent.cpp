// Fill out your copyright notice in the Description page of Project Settings.


#include "Health/PlayerHealthComponent.h"

#include "FMODAudioComponent.h"
#include "FMODBlueprintStatics.h"

void UPlayerHealthComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// For listen server and standalone
	if (GetOwner()->HasAuthority() && GetOwner<APawn>()->IsLocallyControlled())
	{
		UpdateHealthSoundParameter();
	}
}

void UPlayerHealthComponent::BeginPlay()
{
	Super::BeginPlay();

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

void UPlayerHealthComponent::OnRep_CurrentHealth()
{
	Super::OnRep_CurrentHealth();

	// For clients
	UpdateHealthSoundParameter();
}

void UPlayerHealthComponent::UpdateHealthSoundParameter() const
{
	if (GetOwner<APawn>()->IsLocallyControlled() && HealthSoundComponent)
	{
		const float HealthPercentage = GetHealthPercentage();
		HealthSoundComponent->SetParameter(HealthSoundParameterName, HealthPercentage * 100.f);
	}
}
