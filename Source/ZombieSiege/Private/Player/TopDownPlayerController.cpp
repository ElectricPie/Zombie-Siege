// Fill out your copyright notice in the Description page of Project Settings.


#include "TopDownPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PlayerCharacter.h"

void ATopDownPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}
}

void ATopDownPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	FaceMouse();
}

void ATopDownPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATopDownPlayerController::Move);

		// Interaction
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this,
		                                   &ATopDownPlayerController::Interact);

		// Shooting
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &ATopDownPlayerController::Fire);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &ATopDownPlayerController::StopFiring);

		// Weapons
		EnhancedInputComponent->BindAction(SwapWeaponAction, ETriggerEvent::Triggered, this,
		                                   &ATopDownPlayerController::SwapWeapon);
	}
}

void ATopDownPlayerController::Move(const FInputActionValue& Value)
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn()))
	{
		const FVector2D Direction = Value.Get<FVector2D>();
		PlayerCharacter->Move(FVector(Direction.X, Direction.Y, 0.f).GetSafeNormal());
	}
}

void ATopDownPlayerController::FaceMouse()
{
	APlayerCharacter* PlayerActor = Cast<APlayerCharacter>(GetPawn());
	if (PlayerActor == nullptr) return;

	FIntVector2 ViewportSize;
	GetViewportSize(ViewportSize.X, ViewportSize.Y);

	FVector2D MouseScreenLocation;
	if (GetMousePosition(MouseScreenLocation.X, MouseScreenLocation.Y))
	{
		FVector WorldPosition;
		FVector WorldDirection;
		DeprojectScreenPositionToWorld(MouseScreenLocation.X, MouseScreenLocation.Y, WorldPosition, WorldDirection);

		FHitResult HitResult;
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);

		const FVector RayEnd = WorldPosition + WorldDirection * LookRaycastLimit;
		if (GetWorld()->LineTraceSingleByChannel(HitResult, WorldPosition, RayEnd, ECC_Visibility, QueryParams))
		{
			DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 20.f, 8, FColor::Red);
			AimDirection = (HitResult.ImpactPoint - PlayerActor->GetActorLocation()).GetSafeNormal();
			AimDirection.Z = PlayerActor->GetActorLocation().X;
			ClientSetRotation(AimDirection.Rotation());
		}
	}
}

void ATopDownPlayerController::Interact()
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn()))
	{
		PlayerCharacter->Interact();
	}
}

void ATopDownPlayerController::Fire()
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn()))
	{
		PlayerCharacter->Fire(this, AimDirection);
	}
}

void ATopDownPlayerController::StopFiring()
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn()))
	{
		PlayerCharacter->StopFiring();
	}
}

void ATopDownPlayerController::SwapWeapon()
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn()))
	{
		PlayerCharacter->NextWeapon();
	}
}
