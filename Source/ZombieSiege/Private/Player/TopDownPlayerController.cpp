// Fill out your copyright notice in the Description page of Project Settings.


#include "TopDownPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PlayerCharacter.h"

void ATopDownPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
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
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATopDownPlayerController::Move);
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
			//PlayerActor->LookAt(HitResult.ImpactPoint);
			FVector Direction = (HitResult.ImpactPoint - PlayerActor->GetActorLocation()).GetSafeNormal();
			Direction.Z = PlayerActor->GetActorLocation().X;
			ClientSetRotation(Direction.Rotation());
		}
	}
}

