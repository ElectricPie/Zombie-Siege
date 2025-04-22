// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/TopDownPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/WeaponLoadoutComponent.h"
#include "GameFramework/PlayerState.h"
#include "Health/HealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PlayerCharacter.h"
#include "Ui/GameHud.h"

void ATopDownPlayerController::MulticastGameOver_Implementation()
{
	bIsGameOver = true;
	if (AGameHud* GameHud = Cast<AGameHud>(GetHUD()))
	{
		SetShowMouseCursor(true);
		GameHud->ShowGameOver();
		SetInputGameAndUI();
	}
}

void ATopDownPlayerController::SetInputGameOnly()
{
	FInputModeGameOnly InputMode;
	InputMode.SetConsumeCaptureMouseDown(false);
	SetInputMode(InputMode);
	CurrentMouseCursor = EMouseCursor::Crosshairs;
	bShowMouseCursor = true;
	UGameplayStatics::SetViewportMouseCaptureMode(
		GetWorld(), EMouseCaptureMode::CapturePermanently_IncludingInitialMouseDown);
}

void ATopDownPlayerController::SetInputGameAndUI()
{
	FInputModeGameAndUI InputMode;
	// This is need as without it in GameOnly mode it locks the mouse when left-clicking
	InputMode.SetHideCursorDuringCapture(false);
	SetInputMode(InputMode);
	bShowMouseCursor = true;
	CurrentMouseCursor = EMouseCursor::Default;
}

UMoneyStoreComponent* ATopDownPlayerController::GetMoneyStoreComponent_Implementation() const
{
	return IMoneyStoreInterface::Execute_GetMoneyStoreComponent(GetPlayerState<APlayerState>());
}

void ATopDownPlayerController::ClientRespawnPlayer_Implementation()
{
	EnableInput(this);
	SetInputGameOnly();
}

void ATopDownPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (!IsLocalController())
		return;

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}

	// TODO: Setup widget controller for pausing
	if (AGameHud* GameHud = Cast<AGameHud>(GetHUD()))
	{
		GameHud->OnPauseMenuToggledEvent.AddUObject(this, &ATopDownPlayerController::OnPauseMenuChanged);
	}

	SetInputGameAndUI();
	SetInputGameOnly();
}

void ATopDownPlayerController::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!IsLocalController())
		return;

	if (GetPawn<APlayerCharacter>())
	{
		FaceMouse();
	}
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

		// Weapons
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &ATopDownPlayerController::Fire);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this,
		                                   &ATopDownPlayerController::StopFiring);
		EnhancedInputComponent->BindAction(SwapWeaponAction, ETriggerEvent::Triggered, this,
		                                   &ATopDownPlayerController::SwapWeapon);
		EnhancedInputComponent->BindAction(ReloadWeaponAction, ETriggerEvent::Triggered, this,
		                                   &ATopDownPlayerController::ReloadWeapon);

		// Menu
		EnhancedInputComponent->BindAction(MenuAction, ETriggerEvent::Completed, this,
		                                   &ATopDownPlayerController::ToggleMenu);
	}
}

void ATopDownPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (AGameHud* Hud = Cast<AGameHud>(GetHUD()))
	{
		Hud->InitHud();
	}
}

void ATopDownPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	if (AGameHud* Hud = Cast<AGameHud>(GetHUD()))
	{
		Hud->InitHud();
	}
}


void ATopDownPlayerController::AcknowledgePossession(APawn* P)
{
	Super::AcknowledgePossession(P);

	if (const APlayerCharacter* PlayerCharacter = GetPawn<APlayerCharacter>())
	{
		if (const AGameHud* Hud = Cast<AGameHud>(GetHUD()))
		{
			Hud->RebindCharacterWidgetControllerDependencies();
		}

		HealthComponent = PlayerCharacter->GetHealthComponent_Implementation();
		HealthComponent->OnDeathEvent.AddDynamic(this, &ATopDownPlayerController::PlayerDied);
	}
}

void ATopDownPlayerController::Move(const FInputActionValue& Value)
{
	if (!CanDoAction())
		return;

	if (APlayerCharacter* PlayerCharacter = GetPawn<APlayerCharacter>())
	{
		const FVector2D Direction = Value.Get<FVector2D>();
		PlayerCharacter->Move(FVector(Direction.X, Direction.Y, 0.f).GetSafeNormal());
	}
}

void ATopDownPlayerController::FaceMouse()
{
	if (!CanDoAction())
		return;

	if (GetPawn())
	{
		FIntVector2 ViewportSize;
		GetViewportSize(ViewportSize.X, ViewportSize.Y);

		FVector2D MouseScreenLocation;
		if (GetMousePosition(MouseScreenLocation.X, MouseScreenLocation.Y))
		{
			FVector WorldPosition;
			FVector WorldDirection;
			DeprojectScreenPositionToWorld(MouseScreenLocation.X, MouseScreenLocation.Y, WorldPosition, WorldDirection);

			// Z plane intersection for ray
			const float TargetZ = GetPawn()->GetActorLocation().Z;

			FVector AimPoint;
			// Check if ray isn't parallel to the plane
			if (WorldDirection.Z != 0.0f) 
			{
				const float RayDistanceToPlane = (TargetZ - WorldPosition.Z) / WorldDirection.Z;
				// Ensure intersection is in front of camera
				if (RayDistanceToPlane >= 0.0f) 
				{
					AimPoint = WorldPosition + WorldDirection * RayDistanceToPlane;
				}
				else
				{
					// Fallback if behind the camera
					AimPoint = WorldPosition + WorldDirection * LookRaycastLimit;
				}
			}
			else
			{
				// Fallback if parallel to the Z plane
				AimPoint = WorldPosition + WorldDirection * LookRaycastLimit;
			}

			AimLocation = AimPoint;
			const FVector AimDirection = (AimLocation - GetPawn()->GetActorLocation()).GetSafeNormal();
			ClientSetRotation(AimDirection.Rotation());
		}
	}
}

void ATopDownPlayerController::Interact()
{
	if (!CanDoAction())
		return;

	if (const APlayerCharacter* PlayerCharacter = GetPawn<APlayerCharacter>())
	{
		PlayerCharacter->Interact();
	}
}

void ATopDownPlayerController::Fire()
{
	if (!CanDoAction())
		return;

	if (const APlayerCharacter* PlayerCharacter = GetPawn<APlayerCharacter>())
	{
		PlayerCharacter->SetAimLocation(AimLocation);
		GetWorldTimerManager().SetTimer(FireTimerHandle, FTimerDelegate::CreateLambda([this, PlayerCharacter]()
		{
			PlayerCharacter->SetAimLocation(AimLocation);
		}), AimLocationUpdateRate, true);

		PlayerCharacter->Fire();
	}
}

void ATopDownPlayerController::StopFiring()
{
	if (!CanDoAction())
		return;

	if (const APlayerCharacter* PlayerCharacter = GetPawn<APlayerCharacter>())
	{
		StopFireTimer();
		PlayerCharacter->StopFiring();
	}
}

void ATopDownPlayerController::SwapWeapon()
{
	if (!CanDoAction())
		return;

	if (const APlayerCharacter* PlayerCharacter = GetPawn<APlayerCharacter>())
	{
		if (UWeaponLoadoutComponent* WeaponLoadoutComponent = PlayerCharacter->GetWeaponLoadoutComponent())
		{
			StopFireTimer();
			WeaponLoadoutComponent->ServerEquipNextWeapon();
		}
	}
}

void ATopDownPlayerController::ReloadWeapon()
{
	if (!CanDoAction())
		return;

	if (const APlayerCharacter* PlayerCharacter = GetPawn<APlayerCharacter>())
	{
		StopFireTimer();
		PlayerCharacter->ReloadWeapon();
	}
}

bool ATopDownPlayerController::CanDoAction() const
{
	if (bIsGameOver)
		return false;
	if (bIsPaused)
		return false;
	if (HealthComponent.IsValid() && HealthComponent->GetIsDead())
		return false;

	return true;
}

void ATopDownPlayerController::ToggleMenu()
{
	if (AGameHud* GameHud = Cast<AGameHud>(GetHUD()))
	{
		GameHud->ToggleMenu();
	}
}

void ATopDownPlayerController::OnPauseMenuChanged(const bool bMenuIsOpen)
{
	bIsPaused = bMenuIsOpen;

	if (bIsPaused)
	{
		SetInputGameAndUI();
	}
	else
	{
		SetInputGameOnly();
	}
}

void ATopDownPlayerController::PlayerDied(AActor* VictimActor, AController* KillerController, AActor* KillerActor)
{
	DisableInput(this);
	SetInputMode(FInputModeUIOnly());
}

void ATopDownPlayerController::StopFireTimer()
{
	GetWorldTimerManager().ClearTimer(FireTimerHandle);
	FireTimerHandle.Invalidate();
}
