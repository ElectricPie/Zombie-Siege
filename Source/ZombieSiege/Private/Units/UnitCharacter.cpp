// Fill out your copyright notice in the Description page of Project Settings.


#include "Units/UnitCharacter.h"

#include "Components/MoneyRewardComponent.h"
#include "Components/MoneyStoreComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AUnitCharacter::AUnitCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MoneyRewardComponent = CreateDefaultSubobject<UMoneyRewardComponent>(TEXT("Money Reward"));
}

void AUnitCharacter::Attack(AActor* Target)
{
	if (Target == nullptr) return;
	// Delay time between attacks
	if (GetGameTimeSinceCreation() - LastAttackTime < AttackDelay) return;
	UGameplayStatics::ApplyDamage(Target, AttackDamage, GetController(), this, UDamageType::StaticClass());
	LastAttackTime = GetGameTimeSinceCreation();
	PlayAnimMontage(AttackMontage);
}

float AUnitCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	CurrentHealth -= DamageAmount;
	if (CurrentHealth <= 0.f)
	{
		MoneyRewardComponent->RewardMoney(EventInstigator);
		Destroy();
	}
	
	return DamageAmount;
}

void AUnitCharacter::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
}
