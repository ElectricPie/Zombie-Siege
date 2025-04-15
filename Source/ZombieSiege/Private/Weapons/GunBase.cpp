// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/GunBase.h"

#include "FMODBlueprintStatics.h"
#include "FMODEvent.h"
#include "GunProjectile.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Weapons/WeaponStatsDataAsset.h"

// Sets default values
AGunBase::AGunBase()
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

	bReplicates = true;
}

void AGunBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AGunBase, CurrentAmmo, COND_OwnerOnly);
	DOREPLIFETIME(AGunBase, bIsReloading);
}

void AGunBase::Fire()
{
	Fire_Server();
	// TODO: Handle client side prediction
}

void AGunBase::StopFiring()
{
	StopFiring_Server();
	// TODO: Handle client side prediction
}

void AGunBase::Reload()
{
	if (!CanReload())
		return;

	Reload_Server();
}

void AGunBase::SetVisibility(const bool bIsVisible) const
{
	GunMesh->SetVisibility(bIsVisible);
	for (const auto& Child : GunMesh->GetAttachChildren())
	{
		if (Child == ProjectileSpawn)
			continue;

		Child->SetVisibility(bIsVisible);
	}
}

void AGunBase::BeginPlay()
{
	Super::BeginPlay();

	check(WeaponStats);
	CurrentAmmo = WeaponStats->GetMaxAmmo();
	OnAmmoChangedEvent.Broadcast(CurrentAmmo, WeaponStats->GetMaxAmmo());

	// Allow the gun to be fired immediately after spawning
	LastFiredTime = -WeaponStats->GetFireCooldownTime();
}

void AGunBase::Fire_Server_Implementation()
{
	check(GetOwner()->HasAuthority());

	if (bIsReloading)
		return;
	if (bIsFiring)
		return;

	// Prevents firing faster than intended by spam clicking
	if (GetGameTimeSinceCreation() - LastFiredTime < WeaponStats->GetFireCooldownTime())
		return;

	if (CurrentAmmo <= 0)
	{
		MagEmpty();
		return;
	}

	HandleFireMode(nullptr, nullptr);
}

void AGunBase::StopFiring_Server_Implementation()
{
	check(GetOwner()->HasAuthority());

	bIsFiring = false;
	GetWorld()->GetTimerManager().ClearTimer(ShotTimer);
}

void AGunBase::Reload_Server_Implementation()
{
	check(GetOwner()->HasAuthority());

	if (!CanReload())
		return;

	UKismetSystemLibrary::PrintString(this, TEXT("Reloading Server"), true, true, FLinearColor::Blue, 5.f);
	bIsReloading = true;
	OnReloadStateChangedEvent.Broadcast(true);

	if (UFMODEvent* ReloadSound = WeaponStats->GetReloadSound())
	{
		UFMODBlueprintStatics::PlayEventAtLocation(this, ReloadSound, GetActorTransform(), true);
	}

	float ReloadTime = WeaponStats->GetReloadTime();
	if (UAnimMontage* ReloadMontage = WeaponStats->GetReloadAnimMontage())
	{
		ReloadTime = ReloadMontage->GetPlayLength();
		if (ACharacter* Character = Cast<ACharacter>(GetOwner()))
		{
			Character->PlayAnimMontage(ReloadMontage);
		}
	}

	GetWorldTimerManager().SetTimer(ReloadingTimerHandle, FTimerDelegate::CreateLambda([this]()
	{
		CurrentAmmo = WeaponStats->GetMaxAmmo();
		OnAmmoChangedEvent.Broadcast(CurrentAmmo, WeaponStats->GetMaxAmmo());
		OnReloadStateChangedEvent.Broadcast(false);
		bIsReloading = false;
	}), ReloadTime, false);
}

void AGunBase::CancelReload_Server_Implementation()
{
	check(GetOwner()->HasAuthority());

	if (!bIsReloading)
		return;

	GetWorldTimerManager().ClearTimer(ReloadingTimerHandle);

	OnAmmoChangedEvent.Broadcast(CurrentAmmo, WeaponStats->GetMaxAmmo());
	OnReloadStateChangedEvent.Broadcast(false);
	bIsReloading = false;
}

void AGunBase::SpawnProjectile(AController* ShooterController, AActor* ShooterActor)
{
	if (UFMODEvent* FireSound = WeaponStats->GetFireSound())
	{
		UFMODBlueprintStatics::PlayEventAtLocation(this, FireSound, GetActorTransform(), true);
	}

	const FActorSpawnParameters SpawnParameters;
	const FVector SpawnLocation = ProjectileSpawn->GetComponentLocation();

	FRotator SpawnRotation = ProjectileSpawn->GetComponentRotation();
	SpawnRotation.Pitch = 0.f;

	AGunProjectile* Projectile = GetWorld()->SpawnActor<AGunProjectile>(WeaponStats->GetProjectileClass(),
	                                                                    SpawnLocation, SpawnRotation, SpawnParameters);
	Projectile->Init(ShooterController, ShooterActor, WeaponStats->GetProjectileDamageType(),
	                 WeaponStats->GetProjectileDamage());

	OnGunFiredEvent.Broadcast();

	CurrentAmmo--;
	OnAmmoChangedEvent.Broadcast(CurrentAmmo, WeaponStats->GetMaxAmmo());
}

void AGunBase::SingleShot(AController* ShooterController, AActor* ShooterActor)
{
	// Stop shooting if out of ammo
	if (CurrentAmmo <= 0)
	{
		// MagEmpty();
		GetWorld()->GetTimerManager().ClearTimer(ShotTimer);
		return;
	}

	SpawnProjectile(ShooterController, ShooterActor);
}

void AGunBase::BurstShot(AController* ShooterController, AActor* ShooterActor)
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

void AGunBase::HandleFireMode(AController* ShooterController, AActor* ShooterActor)
{
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
			BurstDelegate.BindUFunction(this, GET_FUNCTION_NAME_CHECKED(AGunBase, BurstShot), ShooterController,
			                            ShooterActor);
			GetWorld()->GetTimerManager().SetTimer(BurstTimer, BurstDelegate, WeaponStats->GetShotIntervals(), true);
			break;
		}
	case EGunFireMode::FullAuto:
		{
			SpawnProjectile(ShooterController, ShooterActor);
			FTimerDelegate ShotDelegate;
			ShotDelegate.BindUFunction(this, GET_FUNCTION_NAME_CHECKED(AGunBase, SingleShot), ShooterController,
			                           ShooterActor);
			GetWorld()->GetTimerManager().SetTimer(ShotTimer, ShotDelegate, WeaponStats->GetShotIntervals(), true);
			break;
		}
	default:
		break;
	}
}

void AGunBase::MagEmpty()
{
	if (UFMODEvent* EmptySound = WeaponStats->GetEmptySound())
	{
		UFMODBlueprintStatics::PlayEventAtLocation(this, EmptySound, GetActorTransform(), true);
	}
}

bool AGunBase::CanReload() const
{
	// Prevent reloading if at max ammo
	if (CurrentAmmo >= WeaponStats->GetMaxAmmo())
		return false;

	return true;
}

void AGunBase::OnRep_CurrentAmmo() const
{
	OnAmmoChangedEvent.Broadcast(CurrentAmmo, WeaponStats->GetMaxAmmo());
}

void AGunBase::OnRep_IsReloading() const
{
	OnReloadStateChangedEvent.Broadcast(bIsReloading);

	if (bIsReloading)
	{
		if (UAnimMontage* ReloadMontage = WeaponStats->GetReloadAnimMontage())
		{
			if (ACharacter* Character = Cast<ACharacter>(GetOwner()))
			{
				Character->PlayAnimMontage(ReloadMontage);
			}
		}
	}
}
