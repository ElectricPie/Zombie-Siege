// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyPawn.h"

#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


// Sets default values
ALobbyPawn::ALobbyPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComponent);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	bReplicates = true;
}

void ALobbyPawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyPawn, DesiredMesh);
}

void ALobbyPawn::SetDesiredMesh_Server(USkeletalMesh* NewMesh)
{
	if (!HasAuthority())
		return;

	DesiredMesh = NewMesh;
	Mesh->SetSkeletalMesh(DesiredMesh);
}

void ALobbyPawn::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> CameraActors;
	UGameplayStatics::GetAllActorsOfClass(this, ACameraActor::StaticClass(), CameraActors);
	checkf(CameraActors.Num() > 0, TEXT("No camera actors found in the level, ensure there is a camera actor in the level!"));
	Camera->SetWorldTransform(CameraActors[0]->GetActorTransform());
}

void ALobbyPawn::OnRep_DesiredMesh() const
{
	Mesh->SetSkeletalMesh(DesiredMesh);
}
