// Fill out your copyright notice in the Description page of Project Settings.


#include "Rooms/Room.h"

#include "Barricade.h"

// Sets default values
ARoom::ARoom()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void ARoom::UnlockRoom()
{
	for (const auto& Barricade : Barricades)
	{
		if (!Barricade.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("Barricade on %s room is invalid"), *GetActorNameOrLabel());
			continue;
		}
		Barricade->SetIsActive(true);
	}
}

