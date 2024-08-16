// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerCharacter.h"

#include "TopDownPlayerController.h"
#include "Camera/CameraComponent.h"
#include "Component/HealthComponent.h"
#include "Components/InteractableComponent.h"
#include "Components/InteractorComponent.h"
#include "Components/WeaponLoadoutComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameModes/ZombieDefenceGameMode.h"
#include "Ui/GameHud.h"
#include "Weapons/Gun.h"

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
	InteractorComponent->OnEnterInteractableEvent.AddUObject(this, &APlayerCharacter::OnInteractionEntered);
	InteractorComponent->OnExitInteractableEvent.AddUObject(this, &APlayerCharacter::OnInteractionExited);

	WeaponLoadoutComponent = CreateDefaultSubobject<UWeaponLoadoutComponent>(TEXT("WeaponLoadout"));
	WeaponLoadoutComponent->OnWeaponAddedEvent.AddUObject(this, &APlayerCharacter::OnWeaponAdded);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	HealthComponent->OnDeathEvent.AddDynamic(this, &APlayerCharacter::Die);
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

void APlayerCharacter::Fire(AController* Shooter)
{
	if (bIsReloading) return;
	
	if (AGun* EquippedWeapon = WeaponLoadoutComponent->GetEquippedWeapon())
	{
		EquippedWeapon->StartFiring(Shooter, this);
	}
}

void APlayerCharacter::StopFiring()
{
	if (AGun* EquippedWeapon = WeaponLoadoutComponent->GetEquippedWeapon())
	{
		EquippedWeapon->StopFiring();
	}
}

void APlayerCharacter::ReloadWeapon()
{
	if (AGun* EquippedWeapon = WeaponLoadoutComponent->GetEquippedWeapon())
	{
		if (EquippedWeapon->GetIsReloading()) return;
		
		EquippedWeapon->StopFiring();

		EquippedWeapon->Reload();
		if (UAnimMontage* ReloadAnimation = EquippedWeapon->GetReloadAnimMontage())
		{
			PlayAnimMontage(ReloadAnimation);
		}
	}
}

void APlayerCharacter::OnInteractionEntered(TWeakObjectPtr<UInteractableComponent> InteractableComponent)
{
	if (const APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (AGameHud* GameHud = Cast<AGameHud>(PlayerController->GetHUD()))
		{
			GameHud->SetInteractText(InteractableComponent->GetInteractMessage());
		}
	}
}

void APlayerCharacter::OnInteractionExited(TWeakObjectPtr<UInteractableComponent> InteractableComponent)
{
	if (const APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (AGameHud* GameHud = Cast<AGameHud>(PlayerController->GetHUD()))
		{
			GameHud->HideInteractText();
		}
	}
}

void APlayerCharacter::OnWeaponAdded(AGun* Weapon)
{
	if (Weapon == nullptr) return;

	const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
	Weapon->AttachToComponent(GetMesh(), AttachmentRules, WeaponSocketName);
}

void APlayerCharacter::Die()
{
	if (AZombieDefenceGameMode* GameMode = Cast<AZombieDefenceGameMode>(GetWorld()->GetAuthGameMode()))
	{
		GameMode->PlayerDeath(GetController());
	}

	bIsDead = true;
	OnPlayerDeathEvent.Broadcast(this);
}
