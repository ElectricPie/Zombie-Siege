// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/ZombieDefenceGameMode.h"

#include "Components/MoneyStoreComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Units/UnitSpawnPoint.h"
#include "Units/UnitCharacter.h"

void AZombieDefenceGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Delay getting active spawn points until they are ready
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &AZombieDefenceGameMode::GetActiveUnitSpawnPoints);

		// Initial round setup
		ResetRoundStats();
		OnRoundChangedEvent.Broadcast(RoundNumber);
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

void AZombieDefenceGameMode::OnUnitKilled(TWeakObjectPtr<AUnitCharacter> UnitKilled,
                                          TWeakObjectPtr<AController> KillerInstigator,
                                          TWeakObjectPtr<AActor> KillCauser)
{
	if (!ActiveUnits.Contains(UnitKilled)) return;

	UnitsKilledThisRound++;
	ActiveUnits.Remove(UnitKilled);

	if (UnitsKilledThisRound >= UnitsToBeSpawnedThisRound)
	{
		StartNewRound();
	}
}

int32 AZombieDefenceGameMode::RoundUnitCountBelow20()
{
	return -1.091f + 6.312f * RoundNumber - 0.421f * (RoundNumber * RoundNumber) + 0.013 * (RoundNumber * RoundNumber *
		RoundNumber);
}

int32 AZombieDefenceGameMode::RoundUnitCount20AndAbove()
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
				if (UnitSpawnPoint->GetIsActive())
				{
					ActiveSpawnPoints.Add(UnitSpawnPoint);
				}
				else
				{
					// TODO: Listen for new spawn points becoming active
					// Register for spawn point listener
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
	RoundNumber++;
	OnRoundChangedEvent.Broadcast(RoundNumber);
	
	ResetRoundStats();
	// Get the number of units to be spawned this round
	if (RoundNumber < 20)
	{
		UnitsToBeSpawnedThisRound = RoundUnitCountBelow20();
	}
	else
	{
		UnitsToBeSpawnedThisRound = RoundUnitCount20AndAbove();
	}
	
	GetWorld()->GetTimerManager().SetTimer(RoundSpawnTimerHandle, this, &AZombieDefenceGameMode::SpawnUnit,
										   RoundStartDelay, true, CurrentSpawnDelay);
}

void AZombieDefenceGameMode::ResetRoundStats()
{
	UnitsSpawnedThisRound = 0;
	UnitsKilledThisRound = 0;
}
