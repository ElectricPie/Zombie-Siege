// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/HealthComponent.h"

#include "FMODBlueprintStatics.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;

	if (AActor* Owner = GetOwner())
	{
		Owner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::OnTakeAnyDamage);
	}
}

void UHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
                                       AController* InstigatedBy, AActor* DamageCauser)
{
	CurrentHealth -= Damage;

	if (CurrentHealth <= 0.f)
	{
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

