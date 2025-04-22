// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/WeaponLoadoutComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Health/PlayerHealthComponent.h"
#include "Interactions/InteractorComponent.h"
#include "Weapons/GunBase.h"
#include "Weapons/WeaponStatsDataAsset.h"

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

	WeaponLoadoutComponent = CreateDefaultSubobject<UWeaponLoadoutComponent>(TEXT("WeaponLoadout"));
	WeaponLoadoutComponent->OnWeaponAddedEvent.AddUObject(this, &APlayerCharacter::OnWeaponAdded);

	InteractorComponent = CreateDefaultSubobject<UInteractorComponent>(TEXT("InteractorComponent"));

	HealthComponent = CreateDefaultSubobject<UPlayerHealthComponent>(TEXT("PlayerHealthComponent"));

	GetMesh()->SetReceivesDecals(false);
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

void APlayerCharacter::Interact() const
{
	InteractorComponent->Interact();
}

void APlayerCharacter::Fire() const
{
	if (bIsReloading)
		return;

	WeaponLoadoutComponent->Fire();
}

void APlayerCharacter::StopFiring() const
{
	WeaponLoadoutComponent->StopFiring();
}

void APlayerCharacter::ReloadWeapon() const
{
	WeaponLoadoutComponent->Reload();
}

UMoneyStoreComponent* APlayerCharacter::GetMoneyStoreComponent_Implementation() const
{
	return IMoneyStoreInterface::Execute_GetMoneyStoreComponent(GetController());
}

UHealthComponent* APlayerCharacter::GetHealthComponent_Implementation() const
{
	return HealthComponent;
}

void APlayerCharacter::SetAimLocation(const FVector& NewAimLocation) const
{
	WeaponLoadoutComponent->SetAimLocation(NewAimLocation);
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		OnActorBeginOverlap.AddDynamic(this, &APlayerCharacter::OnOverlap);
		OnActorEndOverlap.AddDynamic(this, &APlayerCharacter::OnOverlapEnd);
		HealthComponent->OnDeathEvent.AddDynamic(this, &APlayerCharacter::Die_Server);
	}

	if (!HasAuthority())
	{
		OnActorBeginOverlap.AddDynamic(this, &APlayerCharacter::OnOverlap);
		OnActorEndOverlap.AddDynamic(this, &APlayerCharacter::OnOverlapEnd);
	}
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

void APlayerCharacter::Die_Server(AActor* VictimActor, AController* KillerController, AActor* KillerActor)
{
	HealthComponent->SetEnableHealthRegen(false);
}

void APlayerCharacter::OnOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	InteractorComponent->OnOverlapBegin(OtherActor);
}

void APlayerCharacter::OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
{
	InteractorComponent->OnOverlapEnd(OtherActor);
}
