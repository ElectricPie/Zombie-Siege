// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/ZombieDefenceGameMode.h"

#include "Components/MoneyStoreComponent.h"
#include "Components/WeaponLoadoutComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/TopDownPlayerController.h"
#include "States/DefenceGameState.h"
#include "Units/UnitSpawnPoint.h"
#include "Units/UnitCharacter.h"
#include "Weapons/Gun.h"

void AZombieDefenceGameMode::PlayerDeath(const AController* PlayerController)
{
	AlivePlayers--;
	
	// Game Over
	if (AlivePlayers <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("All players dead, game over"));
		// Notify all players
		for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
		{
			if (ATopDownPlayerController* TopDownPlayerController = Cast<ATopDownPlayerController>(Iterator->Get()))
			{
				TopDownPlayerController->GameOver();
			}
		}

		// Stop spawning units
		GetWorld()->GetTimerManager().ClearTimer(RoundSpawnTimerHandle);
	}
}

void AZombieDefenceGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Delay getting active spawn points until they are ready
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &AZombieDefenceGameMode::GetActiveUnitSpawnPoints);

		// Initial round setup
		ResetRoundStats();
		UnitsToBeSpawnedThisRound = InitialUnitCount;
		GetWorld()->GetTimerManager().SetTimer(RoundSpawnTimerHandle, this, &AZombieDefenceGameMode::SpawnUnit,
										   RoundStartDelay, true, CurrentSpawnDelay);
	}
}

void AZombieDefenceGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	if (UMoneyStoreComponent* MoneyStore = NewPlayer->GetComponentByClass<UMoneyStoreComponent>())
	{
		MoneyStore->SetMoney(StartingMoney);
	}
}

void AZombieDefenceGameMode::RestartPlayer(AController* NewPlayer)
{
	Super::RestartPlayer(NewPlayer);

	// Gives the player their starting weapons
	if (const AActor* PlayerPawn = NewPlayer->GetPawn())
	{
		if (UWeaponLoadoutComponent* WeaponLoadout = PlayerPawn->FindComponentByClass<UWeaponLoadoutComponent>())
		{
			for (auto& Weapon : StartingWeaponClasses)
			{
				if (Weapon == nullptr) continue;
				
				AGun* NewWeapon = GetWorld()->SpawnActor<AGun>(Weapon);
				WeaponLoadout->AddWeapon(NewWeapon);
			}
		}

		AlivePlayers++;
	}
}

void AZombieDefenceGameMode::OnUnitKilled(TWeakObjectPtr<AUnitCharacter> UnitKilled,
                                          TWeakObjectPtr<AController> KillerInstigator,
                                          TWeakObjectPtr<AActor> KillCauser)
{
	if (!ActiveUnits.Contains(UnitKilled)) return;

	UnitsKilledThisRound++;
	ActiveUnits.Remove(UnitKilled);

	if (UnitsKilledThisRound >= UnitsToBeSpawnedThisRound)
	{
		UE_LOG(LogTemp, Warning, TEXT("All units killed, starting new round"));
		StartNewRound();
	}
}

int32 AZombieDefenceGameMode::RoundUnitCountBelow20(int32 RoundNumber)
{
	return -1.091f + 6.312f * RoundNumber - 0.421f * (RoundNumber * RoundNumber) + 0.013 * (RoundNumber * RoundNumber *
		RoundNumber);
}

int32 AZombieDefenceGameMode::RoundUnitCount20AndAbove(int32 RoundNumber)
{
	return 0.09f * (RoundNumber * RoundNumber) - 0.0029f * RoundNumber + 23.9580;
}

void AZombieDefenceGameMode::GetActiveUnitSpawnPoints()
{
	if (GetWorld())
	{
		ActiveSpawnPoints.Empty();

		TArray<AActor*> UnitSpawnPoints;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AUnitSpawnPoint::StaticClass(), UnitSpawnPoints);
		for (const auto& UnitSpawnPointActor : UnitSpawnPoints)
		{
			if (AUnitSpawnPoint* UnitSpawnPoint = Cast<AUnitSpawnPoint>(UnitSpawnPointActor))
			{
				if (UnitSpawnPoint->GetIsActive() && !UnitSpawnPoint->GetIsForceDeactivated())
				{
					ActiveSpawnPoints.Add(UnitSpawnPoint);
				}
				else
				{
					// Keep track of when a spawn point becomes active
					UnitSpawnPoint->OnActiveStateChangedEvent.AddUObject(this, &AZombieDefenceGameMode::OnSpawnPointActiveChanged);
				}
			}
		}
	}
}

void AZombieDefenceGameMode::SpawnUnit()
{
	if (ActiveSpawnPoints.IsEmpty()) return;
	if (ActiveUnits.Num() >= MaxCurrentSpawnedUnits)
	{
		UE_LOG(LogTemp, Warning, TEXT("Reached max spawned units"));
		return;
	}

	// TODO: Need a weighted spawn point selector as theres is a decent chance with low active spawn points to keep
	// spawning at the same one 
	const int32 SelectedSpawnPoint = UKismetMathLibrary::RandomInteger(ActiveSpawnPoints.Num());
	TWeakObjectPtr<AUnitCharacter> NewUnit = ActiveSpawnPoints[SelectedSpawnPoint]->SpawnUnit(UnitClass);
	
	if (NewUnit.IsValid())
	{
		ActiveUnits.Add(NewUnit);
		NewUnit.Get()->OnKilledEvent.AddUObject(this, &AZombieDefenceGameMode::OnUnitKilled);
		UnitsSpawnedThisRound++;
		if (UnitsSpawnedThisRound >= UnitsToBeSpawnedThisRound)
		{
			GetWorld()->GetTimerManager().ClearTimer(RoundSpawnTimerHandle);
		}
	}
}

void AZombieDefenceGameMode::StartNewRound()
{
	if (ADefenceGameState* DefenceGameState = GetGameState<ADefenceGameState>())
	{
		DefenceGameState->StartNextRound();
		
		ResetRoundStats();
		const int32 CurrentRound = DefenceGameState->GetCurrentRound();
		UE_LOG(LogTemp, Warning, TEXT("New Round %d"), CurrentRound);
		// Get the number of units to be spawned this round
		if (CurrentRound < 20)
		{
			UnitsToBeSpawnedThisRound = RoundUnitCountBelow20(CurrentRound);
		}
		else
		{
			UnitsToBeSpawnedThisRound = RoundUnitCount20AndAbove(CurrentRound);
		}
	
		GetWorld()->GetTimerManager().SetTimer(RoundSpawnTimerHandle, this, &AZombieDefenceGameMode::SpawnUnit,
											   RoundStartDelay, true, CurrentSpawnDelay);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Unable to get DefenceGameState"));
	}
}

void AZombieDefenceGameMode::ResetRoundStats()
{
	UnitsSpawnedThisRound = 0;
	UnitsKilledThisRound = 0;
}

void AZombieDefenceGameMode::OnSpawnPointActiveChanged(TWeakObjectPtr<AUnitSpawnPoint> SpawnPoint, bool bNewActiveState)
{
	if (SpawnPoint.IsValid())
	{
		if (bNewActiveState && !ActiveSpawnPoints.Contains(SpawnPoint))
		{
			ActiveSpawnPoints.Add(SpawnPoint);
		}
		else if (!bNewActiveState && ActiveSpawnPoints.Contains(SpawnPoint))
		{
			ActiveSpawnPoints.Remove(SpawnPoint);
		}
	}
}
