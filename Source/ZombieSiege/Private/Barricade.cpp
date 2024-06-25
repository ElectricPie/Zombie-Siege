// Fill out your copyright notice in the Description page of Project Settings.


#include "Barricade.h"

#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
ABarricade::ABarricade()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	
	OuterTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Outside Trigger"));
	OuterTrigger->SetupAttachment(RootComponent);
	
	InsideTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Inside Trigger"));
	InsideTrigger->SetupAttachment(RootComponent);
	
	InsideDirection = CreateDefaultSubobject<UArrowComponent>(TEXT("Inside Direction Arrow"));
	InsideDirection->SetupAttachment(RootComponent);
}

FVector ABarricade::GetOutsideTriggerPos()
{
	return OuterTrigger->GetComponentLocation();
}

// Called when the game starts or when spawned
void ABarricade::BeginPlay()
{
	Super::BeginPlay();
	
}