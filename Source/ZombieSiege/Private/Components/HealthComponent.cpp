// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HealthComponent.h"

#include "FMODBlueprintStatics.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bEnableHealthRegen)
	{
		const float NewHealth = FMath::Clamp(CurrentHealth + HealthRegenRate * DeltaTime, 0.f, MaxHealth);
		CurrentHealth = NewHealth;
		OnHealthPercentageChangedEvent.Broadcast(GetHealthPercentage());
	}

	// Health Sound
	if (HealthSoundComponent)
	{
		const float HealthPercentage = GetHealthPercentage();
		HealthSoundComponent->SetParameter(HealthSoundParameterName, HealthPercentage * 100.f);
	}
}

void UHealthComponent::SetMaxHealth(const float NewMaxHealth, const bool bKeepHealthPercentage /*=true*/)
{
	if (bKeepHealthPercentage)
	{
		const float HealthPercentage = GetHealthPercentage();
		MaxHealth = NewMaxHealth;
		CurrentHealth = MaxHealth * HealthPercentage;
	}
	else
	{
		MaxHealth = NewMaxHealth;
	}
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;

	if (AActor* Owner = GetOwner())
	{
		Owner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::OnTakeAnyDamage);
	}

	if (HealthSound)
	{
		HealthSoundComponent = UFMODBlueprintStatics::PlayEventAttached(HealthSound,
			GetOwner()->GetRootComponent(),
			NAME_None,
			FVector::ZeroVector,
			EAttachLocation::KeepRelativeOffset,
			true,
			true,
			true);
	}
}

void UHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
                                       AController* InstigatedBy, AActor* DamageCauser)
{
	CurrentHealth -= Damage;

	OnHealthPercentageChangedEvent.Broadcast(GetHealthPercentage());

	if (CurrentHealth <= 0.f)
	{
		// Death
		if (DeathSound && GetOwner())
		{
			UFMODBlueprintStatics::PlayEventAtLocation(GetWorld(), DeathSound, GetOwner()->GetActorTransform(), true);
		}
		
		OnDeathEvent.Broadcast(InstigatedBy, DamageCauser);
	}
	else
	{
		if (HitSound && GetOwner())
		{
			// Only play the sound if one isn't playing already
			if (!HitSoundComponent.IsValid())
			{
				HitSoundComponent = UFMODBlueprintStatics::PlayEventAttached(HitSound,
					GetOwner()->GetRootComponent(),
					NAME_None,
					FVector::ZeroVector,
					EAttachLocation::KeepRelativeOffset,
					true,
					true ,
					true);
			}
		}
	}
}

