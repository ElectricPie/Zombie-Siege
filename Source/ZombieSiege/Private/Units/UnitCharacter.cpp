// Fill out your copyright notice in the Description page of Project Settings.


#include "Units/UnitCharacter.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AUnitCharacter::AUnitCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AUnitCharacter::Attack(AActor* Target)
{
	if (Target == nullptr) return;

	UE_LOG(LogTemp, Warning, TEXT("Attacking"));
	
	UGameplayStatics::ApplyDamage(Target, AttackDamage, GetController(), this, UDamageType::StaticClass());
}
