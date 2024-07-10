// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerCharacter.h"

#include "TopDownPlayerController.h"
#include "Camera/CameraComponent.h"
#include "Components/InteractableComponent.h"
#include "GameFramework/SpringArmComponent.h"
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
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (AGun* EquippedWeapon = GetEquippedWeapon())
	{
		OnWeaponChangedEvent.Broadcast(EquippedWeapon);
	}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	UE_LOG(LogTemp, Warning, TEXT("TakeDamage"));
	
	return 0.f;
}

AGun* APlayerCharacter::GetEquippedWeapon()
{
	if (EquippedWeaponIndex >= Weapons.Num()) return nullptr;

	return Weapons[EquippedWeaponIndex];
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
	// TODO: Remove after weapon animation switching is done
	if (AGun* EquippedWeapon = GetEquippedWeapon())
	{
		OnWeaponChangedEvent.Broadcast(EquippedWeapon);
	}
	
	for (auto const & Interactable : NearbyIntractables)
	{
		Interactable->Interact(this);
	}
}

void APlayerCharacter::Fire(ATopDownPlayerController* Shooter, FVector Direction)
{
	if (AGun* EquippedWeapon = GetEquippedWeapon())
	{
		EquippedWeapon->Fire(Shooter, Direction);
	}
}

void APlayerCharacter::StopFiring()
{
	if (AGun* EquippedWeapon = GetEquippedWeapon())
	{
		EquippedWeapon->StopFiring();
	}
}

void APlayerCharacter::AddInteractable(UInteractableComponent* InteractableComponent)
{
	NearbyIntractables.Add(InteractableComponent);

	if (const APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (AGameHud* GameHud = Cast<AGameHud>(PlayerController->GetHUD()))
		{
			GameHud->SetInteractText(InteractableComponent->GetInteractMessage());
		}
	}
}

void APlayerCharacter::RemoveInteractable(const UInteractableComponent* InteractableComponent)
{
	NearbyIntractables.Remove(InteractableComponent);

	if (const APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (AGameHud* GameHud = Cast<AGameHud>(PlayerController->GetHUD()))
		{
			GameHud->HideInteractText();
		}
	}
}

void APlayerCharacter::NextWeapon()
{
	// Hide the current weapon
	if (AGun* EquippedWeapon = GetEquippedWeapon())
	{
		EquippedWeapon->SetVisibility(false);
	}
	
	if (EquippedWeaponIndex + 1 >= Weapons.Num())
	{
		EquippedWeaponIndex = 0;
	}
	else
	{
		EquippedWeaponIndex++;
	}

	// Show the new weapon
	if (AGun* EquippedWeapon = GetEquippedWeapon())
	{
		OnWeaponChangedEvent.Broadcast(EquippedWeapon);
		EquippedWeapon->SetVisibility(true);
	}
}
