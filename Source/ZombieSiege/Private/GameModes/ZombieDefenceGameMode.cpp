// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/ZombieDefenceGameMode.h"

#include "Ai/UnitAiController.h"
#include "Components/HealthComponent.h"
#include "Components/MoneyRewardComponent.h"
#include "Components/MoneyStoreComponent.h"
#include "Components/WeaponLoadoutComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/TopDownPlayerController.h"
#include "States/DefenceGameState.h"
#include "States/DefencePlayerState.h"
#include "Units/UnitSpawnPoint.h"
#include "Units/UnitCharacter.h"
#include "Weapons/Gun.h"

void AZombieDefenceGameMode::PlayerDeath(const AController* PlayerController)
{
	AlivePlayers--;
	if (ADefencePlayerState* DefencePlayerState = PlayerController->GetPlayerState<ADefencePlayerState>())
	{
		DefencePlayerState->AddDeath();
	}
	
	if (AlivePlayers <= 0)
	{
		GameOver();
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
		StartNewRound();
		GetWorld()->GetTimerManager().SetTimer(RoundSpawnTimerHandle, this, &AZombieDefenceGameMode::SpawnUnit,
										   RoundStartDelay, true, CurrentSpawnDelay);
	}
}

void AZombieDefenceGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	for (auto& PlayerState : GameState->PlayerArray)
	{
		if (UMoneyStoreComponent* MoneyStore = IMoneyStoreInterface::Execute_GetMoneyStoreComponent(PlayerState))
		{
			MoneyStore->SetMoney(StartingMoney);
		}
	}

	// if (UMoneyStoreComponent* MoneyStore = NewPlayer->GetComponentByClass<UMoneyStoreComponent>())
	// {
	// 	MoneyStore->SetMoney(StartingMoney);
	// }
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

void AZombieDefenceGameMode::OnUnitKilled(AUnitCharacter* UnitKilled, AController* KillInstigator, AActor* KillCauser)
{
	if (!ActiveUnits.Contains(UnitKilled)) return;

	UnitsKilledThisRound++;
	ActiveUnits.Remove(UnitKilled);

	if (UnitsKilledThisRound >= UnitsToBeSpawnedThisRound)
	{
		UE_LOG(LogTemp, Warning, TEXT("All units killed, starting new round"));
		StartNewRound();
	}

	if (UMoneyRewardComponent* MoneyRewardComponent = UnitKilled->GetMoneyRewardComponent())
	{
		MoneyRewardComponent->RewardMoney(KillInstigator);
	}

	if (ADefencePlayerState* DefencePlayerState = KillInstigator->GetPlayerState<ADefencePlayerState>())
	{
		DefencePlayerState->AddKill();
	}
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
					FWeightedSpawnPoint* NewWeightedSpawnPoint = new FWeightedSpawnPoint(UnitSpawnPoint, 0.f);
					ActiveSpawnPoints.Add(NewWeightedSpawnPoint);
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

	// Select pawn point
	const TWeakObjectPtr<AUnitSpawnPoint> SelectedSpawnPoint = GetWeightedRandomSpawnPoint();
	if (!SelectedSpawnPoint.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Selected Spawn Point is invalid"));
		return;
	}
	
	// Setup newly spawned unit
	const TWeakObjectPtr<AUnitCharacter> NewUnit = SelectedSpawnPoint->SpawnUnit(UnitClass);
	if (NewUnit.IsValid())
	{
		ActiveUnits.Add(NewUnit);

		if (UHealthComponent* HealthComponent = NewUnit->GetHealthComponent())
		{
			HealthComponent->SetMaxHealth(HealthIncreasePerRound * GetGameState<ADefenceGameState>()->GetCurrentRound());
		}
		
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
		check(ZombieStatsTable);
		const FRealCurve* RoundSpawnCurve = ZombieStatsTable->FindCurve(FName(TEXT("SpawnPerRounds")), FString());
		UnitsToBeSpawnedThisRound = RoundSpawnCurve->Eval(CurrentRound);
	
		UE_LOG(LogTemp, Warning, TEXT("Started round %d | %d Zombies to kill"), CurrentRound, UnitsToBeSpawnedThisRound);

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
		const int32 FoundIndex = ActiveSpawnPoints.IndexOfByPredicate([SpawnPoint](const FWeightedSpawnPoint* SpawnPointWeight)
		{
			return SpawnPointWeight->SpawnPoint == SpawnPoint;
		});
		
		if (bNewActiveState && FoundIndex == INDEX_NONE)
		{
			FWeightedSpawnPoint* NewSpawnPointWeight = new FWeightedSpawnPoint(SpawnPoint, 0.f);
			ActiveSpawnPoints.Add(NewSpawnPointWeight);
		}
		else if (!bNewActiveState && FoundIndex != INDEX_NONE)
		{
			ActiveSpawnPoints.RemoveAt(FoundIndex);
		}
	}
}

void AZombieDefenceGameMode::GameOver()
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

	// Disable all remaining units
	for (const auto& Unit : ActiveUnits)
	{
		if (Unit.IsValid())
		{
			if (AUnitAiController* UnitAiController = Cast<AUnitAiController>(Unit.Get()->GetController()))
			{
				UnitAiController->StopBehaviorTree();
			}
		}
	}
}

TWeakObjectPtr<AUnitSpawnPoint> AZombieDefenceGameMode::GetWeightedRandomSpawnPoint() const
{
	if (ActiveSpawnPoints.IsEmpty())
	{
		return nullptr;
	}
	
	// Get total weight of all spawn points
	float TotalWeight = 0.f;
	for (const auto& WeightedSpawnPoint : ActiveSpawnPoints)
	{
		if (!WeightedSpawnPoint->SpawnPoint.IsValid())
		{
			continue;
		}
	
		TotalWeight += GetWorld()->TimeSince(WeightedSpawnPoint->LastUsedTime); 
	}
	
	// Get random spawn point
	const float RandomWeight = UKismetMathLibrary::RandomFloatInRange(0.f, TotalWeight);
	for (const auto& WeightedSpawnPoint : ActiveSpawnPoints)
	{
		if (!WeightedSpawnPoint->SpawnPoint.IsValid())
		{
			continue;
		}
	
		TotalWeight -= GetWorld()->TimeSince(WeightedSpawnPoint->LastUsedTime);
		if (TotalWeight <= RandomWeight)
		{
			WeightedSpawnPoint->LastUsedTime = GetWorld()->GetTimeSeconds();
			return WeightedSpawnPoint->SpawnPoint;
		}
	}
	
	ActiveSpawnPoints[0]->LastUsedTime = GetWorld()->GetTimeSeconds();
	return ActiveSpawnPoints[0]->SpawnPoint;
}
