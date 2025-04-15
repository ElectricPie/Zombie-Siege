// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerCharacter.h"

#include "TopDownPlayerController.h"
#include "Camera/CameraComponent.h"
#include "Components/HealthComponent.h"
#include "Interactions/InteractorComponent.h"
#include "Components/WeaponLoadoutComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameModes/ZombieDefenceGameMode.h"
#include "Weapons/GunBase.h"
#include "ZombieSiege/Public/Weapons/WeaponStatsDataAsset.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	CameraArm->SetupAttachment(RootComponent);
	CameraArm->SetUsingAbsoluteRotation(true);
	CameraArm->TargetArmLength = 1200.f;
	CameraArm->SetRelativeRotation(FRotator(-70.f, 0.f, 0.f));
	CameraArm->bDoCollisionTest = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraArm);
	Camera->bUsePawnControlRotation = false;

	InteractorComponent = CreateDefaultSubobject<UInteractorComponent>(TEXT("Interactor"));

	WeaponLoadoutComponent = CreateDefaultSubobject<UWeaponLoadoutComponent>(TEXT("WeaponLoadout"));
	WeaponLoadoutComponent->OnWeaponAddedEvent.AddUObject(this, &APlayerCharacter::OnWeaponAdded);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	HealthComponent->OnDeathEvent.AddDynamic(this, &APlayerCharacter::Die);

	GetMesh()->SetReceivesDecals(false);
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

bool APlayerCharacter::IsMovingForward() const
{
	const FVector ForwardVector = GetActorForwardVector();
	const FVector Velocity = GetVelocity().GetSafeNormal();

	const float DotProduct = FVector::DotProduct(ForwardVector, Velocity);
	return DotProduct > BackwardsThreshold;
}

void APlayerCharacter::Move(const FVector Direction)
{
	AddMovementInput(FVector::ForwardVector, Direction.X * SpeedModifier);
	AddMovementInput(FVector::RightVector, Direction.Y * SpeedModifier);
}

void APlayerCharacter::Interact()
{
	InteractorComponent->Interact();
}

void APlayerCharacter::Fire(AController* Shooter) const
{
	if (bIsReloading)
		return;

	WeaponLoadoutComponent->Fire();
}

void APlayerCharacter::StopFiring()
{
	WeaponLoadoutComponent->StopFiring();
}

void APlayerCharacter::ReloadWeapon()
{
	WeaponLoadoutComponent->Reload();
}

UMoneyStoreComponent* APlayerCharacter::GetMoneyStoreComponent_Implementation() const
{
	return IMoneyStoreInterface::Execute_GetMoneyStoreComponent(GetController());
}

void APlayerCharacter::OnWeaponAdded(AGunBase* Weapon)
{
	if (Weapon == nullptr)
		return;

	const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);

	FName WeaponSocketName;
	switch (Weapon->GetWeaponStats()->GetGunType())
	{
	case EGunType::Pistol:
		WeaponSocketName = PistolWeaponSocket;
		break;
	case EGunType::Rifle:
		WeaponSocketName = RifleWeaponSocket;
		break;
	default:
		return;
	}
	Weapon->AttachToComponent(GetMesh(), AttachmentRules, WeaponSocketName);
}

void APlayerCharacter::Die(AController* KillInstigator, AActor* KillCauser)
{
	if (AZombieDefenceGameMode* GameMode = Cast<AZombieDefenceGameMode>(GetWorld()->GetAuthGameMode()))
	{
		GameMode->PlayerDeath(GetController());
	}

	bIsDead = true;
	if (HealthComponent)
	{
		HealthComponent->SetEnableHealthRegen(false);
	}

	OnPlayerDeathEvent.Broadcast(this);
}
