// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Gun.h"

#include "GunProjectile.h"
#include "Components/ArrowComponent.h"
#include "Player/PlayerCharacter.h"
#include "Player/TopDownPlayerController.h"

// Sets default values
AGun::AGun()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun Mesh"));
	GunMesh->SetupAttachment(RootComponent);

	ProjectileSpawn = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn"));
	ProjectileSpawn->SetupAttachment(GunMesh);

	FiringArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Firing Direction"));
	FiringArrow->SetupAttachment(RootComponent);
}

// TODO: Aim direction is only set once so it always shoots the same direction
void AGun::Fire(ATopDownPlayerController* Shooter)
{
	if (bIsFiring || Shooter == nullptr) return;
	bIsFiring = true;

	switch (FireRate)
	{
	case Single:
		SpawnProjectile(Shooter);
		break;
	case Burst:
		{
			SpawnProjectile(Shooter);
			BurstShotsFired = 1;
			FTimerDelegate BurstDelegate;
			BurstDelegate.BindUFunction(this, FName(TEXT("BurstShot")), Shooter);
			GetWorld()->GetTimerManager().SetTimer(BurstTimer, BurstDelegate, ShotIntervals, true);
			
			break;
		}
	case FullAuto:
		{
			SpawnProjectile(Shooter);
			FTimerDelegate ShotDelegate;
			ShotDelegate.BindUFunction(this, FName(TEXT("SpawnProjectile")), Shooter);
			GetWorld()->GetTimerManager().SetTimer(ShotTimer, ShotDelegate, ShotIntervals, true);
			break;
		}
	default:
		UE_LOG(LogTemp, Error, TEXT("%s is missing Fire Rate"), *GetActorNameOrLabel());
		break;
	}
}

void AGun::StopFiring()
{
	bIsFiring = false;
	GetWorld()->GetTimerManager().ClearTimer(ShotTimer);
}

void AGun::SetVisibility(const bool bIsVisible)
{
	GunMesh->SetVisibility(bIsVisible);
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGun::SpawnProjectile(ATopDownPlayerController* Shooter)
{
	if (!ProjectileClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s is missing projectile class"), *GetActorNameOrLabel());
		return;
	}

	const FActorSpawnParameters SpawnParameters;
	const FVector SpawnLocation = ProjectileSpawn->GetComponentLocation();
	FRotator SpawnRotation = Shooter->GetAimDirection().Rotation();
	SpawnRotation.Pitch = 0.f;

	AGunProjectile* Projectile = GetWorld()->SpawnActor<AGunProjectile>(ProjectileClass, SpawnLocation, SpawnRotation,
	                                                                    SpawnParameters);
	Projectile->Shooter = Shooter;
	Projectile->Damage = ProjectileDamage;
	Projectile->DamageType = ProjectileDamageType;
}

void AGun::BurstShot(ATopDownPlayerController* Shooter)
{
	SpawnProjectile(Shooter);
	BurstShotsFired++;

	// Stop the burst
	if (BurstShotsFired >= BurstShots)
	{
		GetWorld()->GetTimerManager().ClearTimer(BurstTimer);
	}
}