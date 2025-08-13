// Fill out your copyright notice in the Description page of Project Settings.


#include "Health/HealthComponent.h"

#include "FMODBlueprintStatics.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	SetIsReplicatedByDefault(true);
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHealthComponent, MaxHealth);
	DOREPLIFETIME(UHealthComponent, CurrentHealth);
	DOREPLIFETIME(UHealthComponent, bIsDead);
}

void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (GetOwner()->HasAuthority() && bEnableHealthRegen)
	{
		const float NewHealth = FMath::Clamp(CurrentHealth + HealthRegenRate * DeltaTime, 0.f, MaxHealth);
		CurrentHealth = NewHealth;
		OnHealthPercentageChangedEvent.Broadcast(GetHealthPercentage());
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

	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::OnTakeAnyDamage);
}

void UHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
                                       AController* InstigatedBy, AActor* DamageCauser)
{
	if (!GetOwner()->HasAuthority())
		return;

	if (bIsDead)
		return;

	CurrentHealth -= Damage;
	OnHealthPercentageChangedEvent.Broadcast(GetHealthPercentage());

	if (CurrentHealth <= 0.f)
	{
		bIsDead = true;
		MulticastDie(InstigatedBy, DamageCauser);
	}
	else
	{
		MulticastHit();
	}
}

void UHealthComponent::OnRep_CurrentHealth()
{
	OnHealthPercentageChangedEvent.Broadcast(GetHealthPercentage());
	OnCurrentHealthChangedEvent.Broadcast(GetHealthPercentage());
}

void UHealthComponent::MulticastHit_Implementation()
{
	if (HitSound)
	{
		UFMODBlueprintStatics::PlayEventAtLocation(GetWorld(), HitSound, GetOwner()->GetActorTransform(), true);
	}
}

void UHealthComponent::MulticastDie_Implementation(AController* KillerController, AActor* KillerActor)
{
	if (DeathSound)
	{
		UFMODBlueprintStatics::PlayEventAtLocation(GetWorld(), DeathSound, GetOwner()->GetActorTransform(), true);
	}

	OnDeathEvent.Broadcast(GetOwner(), KillerController, KillerActor);
}
