// Fill out your copyright notice in the Description page of Project Settings.


#include "Units/UnitCharacter.h"

#include "FMODBlueprintStatics.h"
#include "Components/CapsuleComponent.h"
#include "Health/HealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Money/MoneyRewardComponent.h"

// Sets default values
AUnitCharacter::AUnitCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MoneyRewardComponent = CreateDefaultSubobject<UMoneyRewardComponent>(TEXT("Money Reward"));
	
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
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

UHealthComponent* AUnitCharacter::GetHealthComponent_Implementation() const
{
	return HealthComponent;
}

void AUnitCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		HealthComponent->OnDeathEvent.AddDynamic(this, &AUnitCharacter::Die_Server);
	}
	else
	{
		HealthComponent->OnCurrentHealthChangedEvent.AddUObject(this, &AUnitCharacter::HealthChange_Client);
	}
}

void AUnitCharacter::Die_Server(AController* KillInstigator, AActor* KillCauser)
{
	Ragdoll();
	
	OnKilledEvent.Broadcast(this, KillInstigator, KillCauser);
	SetLifeSpan(DeathLifeSpan);
}

void AUnitCharacter::HealthChange_Client(const float NewCurrentHealth)
{
	if (NewCurrentHealth <= 0.f)
	{
		Ragdoll();
	}
}

void AUnitCharacter::Ragdoll()
{
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

