// Fill out your copyright notice in the Description page of Project Settings.


#include "Units/UnitCharacter.h"

#include "FMODBlueprintStatics.h"
#include "Components/HealthComponent.h"
#include "Components/MoneyRewardComponent.h"
#include "Rooms/Barricade.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AUnitCharacter::AUnitCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MoneyRewardComponent = CreateDefaultSubobject<UMoneyRewardComponent>(TEXT("Money Reward"));
	
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
	HealthComponent->OnDeathEvent.AddDynamic(this, &AUnitCharacter::Die);
}

void AUnitCharacter::Attack(AActor* Target)
{
	if (Target == nullptr) return;
	// Delay time between attacks
	if (GetGameTimeSinceCreation() - LastAttackTime < AttackDelay) return;
	UGameplayStatics::ApplyDamage(Target, AttackDamage, GetController(), this, UDamageType::StaticClass());
	LastAttackTime = GetGameTimeSinceCreation();

	if (AttackSound)
	{
		UFMODBlueprintStatics::PlayEventAtLocation(GetWorld(), AttackSound, GetActorTransform(), true);
	}
	
	if (AttackMontage)
	{
		PlayAnimMontage(AttackMontage);
	}
}

void AUnitCharacter::SetTargetBarricade(TWeakObjectPtr<ABarricade> NewTargetBarricade)
{
	TargetBarricade = NewTargetBarricade;
}

void AUnitCharacter::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
}

void AUnitCharacter::Die(AController* KillInstigator, AActor* KillCauser)
{
	OnKilledEvent.Broadcast(this, KillInstigator, KillCauser);
	Destroy();
}

