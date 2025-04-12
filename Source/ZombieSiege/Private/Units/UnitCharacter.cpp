// Fill out your copyright notice in the Description page of Project Settings.


#include "Units/UnitCharacter.h"

#include "FMODBlueprintStatics.h"
#include "Components/CapsuleComponent.h"
#include "Components/HealthComponent.h"
#include "Components/MoneyRewardComponent.h"
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
	if (Target == nullptr)
		return;
	// Delay time between attacks
	if (GetGameTimeSinceCreation() - LastAttackTime < AttackDelay)
		return;
	
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

void AUnitCharacter::SetTargetBarricade(ABarricade* NewTargetBarricade)
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
	// Ragdoll the unit
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	OnKilledEvent.Broadcast(this, KillInstigator, KillCauser);
	SetLifeSpan(DeathLifeSpan);
}

