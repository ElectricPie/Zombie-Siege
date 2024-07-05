// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/InteractableComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Ui/GameHud.h"

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

	UE_LOG(LogTemp, Warning, TEXT("Runnnign"));
	
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

void APlayerCharacter::LookAt(const FVector Pos)
{
	const FRotator Direction = (GetActorLocation() - Pos).Rotation();
	DrawDebugLine(GetWorld(), GetActorLocation(), Pos, FColor::Green);
	//SetActorRotation(Direction);
}

void APlayerCharacter::Interact()
{
	for (auto const & Interactable : NearbyIntractables)
	{
		Interactable->Interact(this);
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
