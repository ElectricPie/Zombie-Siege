// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Gun.h"

#include "FMODBlueprintStatics.h"
#include "FMODEvent.h"
#include "GunProjectile.h"
#include "Components/ArrowComponent.h"
#include "Player/TopDownPlayerController.h"
#include "ZombieSiege/Public/Weapons/WeaponStatsDataAsset.h"

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

void AGun::StartFiring(AController* ShooterController, AActor* ShooterActor)
{
	if (ShooterController == nullptr)
		return;
	if (bIsFiring || bIsReloading)
		return;
	
	if (CurrentAmmo <= 0)
	{
		MagEmpty();
		return;
	}
	if (GetGameTimeSinceCreation() - LastFiredTime < WeaponStats->GetFireCooldownTime())
		return;
	
	bIsFiring = true;
	LastFiredTime = GetGameTimeSinceCreation();

	// Handle different fire modes
	switch (GetWeaponStats()->GetFireMode())
	{
		case EGunFireMode::Single:
			SpawnProjectile(ShooterController, ShooterActor);
			break;
		case EGunFireMode::Burst:
			{
				SpawnProjectile(ShooterController, ShooterActor);
				BurstShotsFired = 1;
				FTimerDelegate BurstDelegate;
				BurstDelegate.BindUFunction(this, GET_FUNCTION_NAME_CHECKED(AGun, BurstShot), ShooterController, ShooterActor);
				GetWorld()->GetTimerManager().SetTimer(BurstTimer, BurstDelegate, WeaponStats->GetShotIntervals(), true);
				break;
			}
		case EGunFireMode::FullAuto:
			{
				SpawnProjectile(ShooterController, ShooterActor);
				FTimerDelegate ShotDelegate;
				ShotDelegate.BindUFunction(this, GET_FUNCTION_NAME_CHECKED(AGun, SingleShot), ShooterController, ShooterActor);
				GetWorld()->GetTimerManager().SetTimer(ShotTimer, ShotDelegate, WeaponStats->GetShotIntervals(), true);
				break;
			}
		default:
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
	for (const auto& Child : GunMesh->GetAttachChildren())
	{
		if (Child == ProjectileSpawn)
			continue;
		
		Child->SetVisibility(bIsVisible);
	}
}

void AGun::Reload()
{
	bIsReloading = true;
	OnReloadStateChangedEvent.Broadcast(true);

	if (UFMODEvent* ReloadSound = WeaponStats->GetReloadSound())
	{
		UFMODBlueprintStatics::PlayEventAtLocation(this, ReloadSound, GetActorTransform(), true);
	}
	
	float ReloadTime = WeaponStats->GetReloadTime();
	if (const UAnimMontage* ReloadMontage = WeaponStats->GetReloadAnimMontage())
	{
		ReloadTime = ReloadMontage->GetPlayLength();
	}
	GetWorldTimerManager().SetTimer(ReloadingTimerHandle, this, &AGun::FinishReload, ReloadTime, false);
}

void AGun::CancelReload()
{
	if (!bIsReloading)
		return;

	GetWorldTimerManager().ClearTimer(ReloadingTimerHandle);

	UE_LOG(LogTemp, Warning, TEXT("Canceled Reload"));
	
	OnAmmoChangedEvent.Broadcast(CurrentAmmo, WeaponStats->GetMaxAmmo());
	OnReloadStateChangedEvent.Broadcast(false);
	bIsReloading = false;
}

void AGun::BeginPlay()
{
	Super::BeginPlay();

	check(WeaponStats);
	CurrentAmmo = WeaponStats->GetMaxAmmo();
	OnAmmoChangedEvent.Broadcast(CurrentAmmo, WeaponStats->GetMaxAmmo());
	
	// Allow the gun to be fired immediately after spawning
	LastFiredTime = -WeaponStats->GetFireCooldownTime();
}

void AGun::SpawnProjectile(AController* ShooterController, AActor* ShooterActor)
{
	if (UFMODEvent* FireSound = WeaponStats->GetFireSound())
	{
		UFMODBlueprintStatics::PlayEventAtLocation(this, FireSound, GetActorTransform(), true);
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

	AGunProjectile* Projectile = GetWorld()->SpawnActor<AGunProjectile>(WeaponStats->GetProjectileClass(), SpawnLocation, SpawnRotation,
	                                                                    SpawnParameters);
	Projectile->Init(ShooterController, ShooterActor, WeaponStats->GetProjectileDamageType(), WeaponStats->GetProjectileDamage());

	OnGunFiredEvent.Broadcast();
	
	CurrentAmmo--;
	OnAmmoChangedEvent.Broadcast(CurrentAmmo, WeaponStats->GetMaxAmmo());
}

void AGun::SingleShot(AController* ShooterController, AActor* ShooterActor)
{
	// Stop shooting if out of ammo
	if (CurrentAmmo <= 0)
	{
		MagEmpty();
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
		MagEmpty();
		GetWorld()->GetTimerManager().ClearTimer(BurstTimer);
		return;
	}
	
	SpawnProjectile(ShooterController, ShooterActor);
	BurstShotsFired++;

	// Stop the burst if finished
	if (BurstShotsFired >= WeaponStats->GetBurstShots())
	{
		GetWorld()->GetTimerManager().ClearTimer(BurstTimer);
	}
}

void AGun::FinishReload()
{
	UE_LOG(LogTemp, Warning, TEXT("Reload Finished"));
	
	CurrentAmmo = WeaponStats->GetMaxAmmo();
	OnAmmoChangedEvent.Broadcast(CurrentAmmo, WeaponStats->GetMaxAmmo());
	OnReloadStateChangedEvent.Broadcast(false);
	bIsReloading = false;
}

void AGun::MagEmpty()
{
	if (UFMODEvent* EmptySound = WeaponStats->GetEmptySound())
	{
		UFMODBlueprintStatics::PlayEventAtLocation(this, EmptySound, GetActorTransform(), true);
	}
}
