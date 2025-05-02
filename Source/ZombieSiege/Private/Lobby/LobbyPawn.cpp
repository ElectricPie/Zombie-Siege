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
	HeadGearMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HeadGearMesh"));
	HeadGearMesh->SetupAttachment(Mesh, TEXT("HeadGearSocket"));

	bReplicates = true;
}

void ALobbyPawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyPawn, DesiredCharacterMesh);
	DOREPLIFETIME(ALobbyPawn, DesiredHeadGearMesh);
}

void ALobbyPawn::SetDesiredCharacterMesh_Server(USkeletalMesh* NewMesh)
{
	check(HasAuthority());

	DesiredCharacterMesh = NewMesh;
	Mesh->SetSkeletalMesh(DesiredCharacterMesh);
}

void ALobbyPawn::SetDesiredHeadGearMesh_Server(UStaticMesh* NewMesh)
{
	check(HasAuthority());

	DesiredHeadGearMesh = NewMesh;
	HeadGearMesh->SetStaticMesh(DesiredHeadGearMesh);
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
	Mesh->SetSkeletalMesh(DesiredCharacterMesh);
}

void ALobbyPawn::OnRep_DesiredHeadGearMesh() const
{
	HeadGearMesh->SetStaticMesh(DesiredHeadGearMesh);
}
