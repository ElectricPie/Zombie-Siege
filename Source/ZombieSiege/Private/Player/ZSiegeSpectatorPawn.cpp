// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ZSiegeSpectatorPawn.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"


// Sets default values
AZSiegeSpectatorPawn::AZSiegeSpectatorPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	CameraArm->SetupAttachment(RootComponent);
	CameraArm->SetUsingAbsoluteRotation(true);
	CameraArm->TargetArmLength = 1200.f;
	CameraArm->SetRelativeRotation(FRotator(-70.f, 0.f, 0.f));
	CameraArm->bDoCollisionTest = false;
	CameraArm->bEnableCameraLag = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraArm);
	Camera->bUsePawnControlRotation = false;

	bReplicates = true;
}

void AZSiegeSpectatorPawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AZSiegeSpectatorPawn, FollowTarget, COND_OwnerOnly);
}

void AZSiegeSpectatorPawn::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (IsLocallyControlled() && FollowTarget.IsValid())
	{
		SetActorLocation(FollowTarget.Get()->GetActorLocation());
	}
}

void AZSiegeSpectatorPawn::SetFollowTarget(AActor* NewTarget)
{
	FollowTarget = NewTarget;
}
