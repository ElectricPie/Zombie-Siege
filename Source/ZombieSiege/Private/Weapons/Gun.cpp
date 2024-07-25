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
	PrimaryActorTick.bCanEverTick = false;

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
void AGun::StartFiring(AController* ShooterController, AActor* ShooterActor)
{
	if (ShooterController == nullptr) return;
	if (bIsFiring || bIsReloading) return;
	if (CurrentAmmo <= 0) return;
	if (GetGameTimeSinceCreation() - LastFiredTime < FireCooldownTime) return;
	
	bIsFiring = true;
	LastFiredTime = GetGameTimeSinceCreation();

	switch (FireRate)
	{
	case Single:
		SpawnProjectile(ShooterController, ShooterActor);
		break;
	case Burst:
		{
			SpawnProjectile(ShooterController, ShooterActor);
			BurstShotsFired = 1;
			FTimerDelegate BurstDelegate;
			BurstDelegate.BindUFunction(this, GET_FUNCTION_NAME_CHECKED(AGun, BurstShot), ShooterController);
			GetWorld()->GetTimerManager().SetTimer(BurstTimer, BurstDelegate, ShotIntervals, true);
			break;
		}
	case FullAuto:
		{
			SpawnProjectile(ShooterController, ShooterActor);
			FTimerDelegate ShotDelegate;
			ShotDelegate.BindUFunction(this,  GET_FUNCTION_NAME_CHECKED(AGun, SingleShot), ShooterController);
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

void AGun::Reload()
{
	bIsReloading = true;
	OnReloadStateChangedEvent.Broadcast(true);
	
	float ReloadTime = DefaultReloadTime;
	if (ReloadMontage)
	{
		ReloadTime = ReloadMontage->GetPlayLength();
	}
	GetWorldTimerManager().SetTimer(ReloadingTimerHandle, this, &AGun::FinishReload, ReloadTime, false);
}


void AGun::PostInitProperties()
{
	Super::PostInitProperties();

	CurrentAmmo = MaxAmmo;
}

void AGun::SpawnProjectile(AController* ShooterController, AActor* ShooterActor)
{
	if (!ProjectileClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s is missing projectile class"), *GetActorNameOrLabel());
		return;
	}

	const FActorSpawnParameters SpawnParameters;
	const FVector SpawnLocation = ProjectileSpawn->GetComponentLocation();

	// Need to get the aim direction of the shooter at is changed between shots
	FRotator SpawnRotation;
	if (const ATopDownPlayerController* TopDownController = Cast<ATopDownPlayerController>(ShooterController))
	{
		SpawnRotation = TopDownController->GetAimDirection().Rotation();
	}
	else
	{
		SpawnRotation = ShooterActor->GetActorForwardVector().Rotation();
	}
	SpawnRotation.Pitch = 0.f;

	AGunProjectile* Projectile = GetWorld()->SpawnActor<AGunProjectile>(ProjectileClass, SpawnLocation, SpawnRotation,
	                                                                    SpawnParameters);
	Projectile->Init(ShooterController, ShooterActor, ProjectileDamageType, ProjectileDamage);
	
	CurrentAmmo--;
	OnAmmoChangedEvent.Broadcast(CurrentAmmo, MaxAmmo);
}

void AGun::SingleShot(AController* ShooterController, AActor* ShooterActor)
{
	// Stop shooting if out of ammo
	if (CurrentAmmo <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(ShotTimer);
		return;
	}

	SpawnProjectile(ShooterController, ShooterActor);
}

void AGun::BurstShot(AController* ShooterController, AActor* ShooterActor)
{
	// Stop burst if out of ammo
	if (CurrentAmmo <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(BurstTimer);
		return;
	}
	
	SpawnProjectile(ShooterController, ShooterActor);
	BurstShotsFired++;

	// Stop the burst if finished
	if (BurstShotsFired >= BurstShots)
	{
		GetWorld()->GetTimerManager().ClearTimer(BurstTimer);
	}
}

void AGun::FinishReload()
{
	CurrentAmmo = MaxAmmo;
	OnAmmoChangedEvent.Broadcast(CurrentAmmo, MaxAmmo);
	OnReloadStateChangedEvent.Broadcast(false);
	bIsReloading = false;
}
