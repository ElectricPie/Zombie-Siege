// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Gun.h"

#include "GunProjectile.h"
#include "Components/ArrowComponent.h"

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

void AGun::Fire(ATopDownPlayerController* Shooter)
{
	if (!ProjectileClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s is missing projectile class"), *GetActorNameOrLabel());
		return;
	}

	const FActorSpawnParameters SpawnParameters;
	const FVector SpawnLocation = ProjectileSpawn->GetComponentLocation();
	FRotator SpawnRotation = GetActorRotation();
	SpawnRotation.Pitch = 0.f;

	AGunProjectile* Projectile = GetWorld()->SpawnActor<AGunProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParameters);
	Projectile->Shooter = Shooter;
	Projectile->Damage = ProjectileDamage;
	Projectile->DamageType = ProjectileDamageType;
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

