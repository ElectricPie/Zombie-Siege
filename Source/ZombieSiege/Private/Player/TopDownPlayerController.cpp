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
	UGameplayStatics::SetViewportMouseCaptureMode(GetWorld(), EMouseCaptureMode::CapturePermanently_IncludingInitialMouseDown);
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

void ATopDownPlayerController::PlayerRespawned()
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
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ATopDownPlayerController::Interact);

		// Weapons
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &ATopDownPlayerController::Fire);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &ATopDownPlayerController::StopFiring);
		EnhancedInputComponent->BindAction(SwapWeaponAction, ETriggerEvent::Triggered, this, &ATopDownPlayerController::SwapWeapon);
		EnhancedInputComponent->BindAction(ReloadWeaponAction, ETriggerEvent::Triggered, this, &ATopDownPlayerController::ReloadWeapon);

		// Menu
		EnhancedInputComponent->BindAction(MenuAction, ETriggerEvent::Completed, this, &ATopDownPlayerController::ToggleMenu);
	}
}

void ATopDownPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (AGameHud* Hud = Cast<AGameHud>(GetHUD()))
	{
		Hud->InitHud();
	}

	const APlayerCharacter* PlayerCharacter = GetPawn<APlayerCharacter>();
	HealthComponent = PlayerCharacter->GetHealthComponent_Implementation();
	HealthComponent->OnDeathEvent.AddDynamic(this, &ATopDownPlayerController::PlayerDied);
}

void ATopDownPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	if (AGameHud* Hud = Cast<AGameHud>(GetHUD()))
	{
		Hud->InitHud();
	}

	const APlayerCharacter* PlayerCharacter = GetPawn<APlayerCharacter>();
	HealthComponent = PlayerCharacter->GetHealthComponent_Implementation();
	HealthComponent->OnDeathEvent.AddDynamic(this, &ATopDownPlayerController::PlayerDied);
}

void ATopDownPlayerController::Move(const FInputActionValue& Value)
{
	if (!CanDoAction())
		return;

	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn()))
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

			FHitResult HitResult;
			FCollisionQueryParams QueryParams;
			QueryParams.AddIgnoredActor(this);

			const FVector RayEnd = WorldPosition + WorldDirection * LookRaycastLimit;
			if (GetWorld()->LineTraceSingleByChannel(HitResult, WorldPosition, RayEnd, ECC_Visibility, QueryParams))
			{
				AimDirection = (HitResult.ImpactPoint - GetPawn()->GetActorLocation()).GetSafeNormal();
				AimDirection.Z = GetPawn()->GetActorLocation().X;
				ClientSetRotation(AimDirection.Rotation());
			}
		}
	}
}

void ATopDownPlayerController::Interact()
{
	if (!CanDoAction())
		return;

	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn()))
	{
		PlayerCharacter->Interact();
	}
}

void ATopDownPlayerController::Fire()
{
	if (!CanDoAction())
		return;
	
	if (const APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn()))
	{
		PlayerCharacter->Fire();
	}
}

void ATopDownPlayerController::StopFiring()
{
	if (!CanDoAction())
		return;

	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn()))
	{
		PlayerCharacter->StopFiring();
	}
}

void ATopDownPlayerController::SwapWeapon()
{
	if (!CanDoAction())
		return;

	if (const APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn()))
	{
		if (UWeaponLoadoutComponent* WeaponLoadoutComponent = PlayerCharacter->GetWeaponLoadoutComponent())
		{
			WeaponLoadoutComponent->ServerEquipNextWeapon();
		}
	}
}

void ATopDownPlayerController::ReloadWeapon()
{
	if (!CanDoAction())
		return;

	if (const APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn()))
	{
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

	if (const IHealthComponentInterface* HealthComponentInterface = Cast<APlayerCharacter>(GetPawn()))
	{
		if (HealthComponentInterface->GetHealthComponent_Implementation()->GetIsDead())
			return false;
	}

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
