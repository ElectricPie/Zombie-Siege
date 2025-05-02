// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "LobbyPawn.generated.h"

class UCameraComponent;

UCLASS()
class ZOMBIESIEGE_API ALobbyPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ALobbyPawn();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	void SetDesiredMesh_Server(USkeletalMesh* NewMesh);

protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> Camera;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> Mesh;
	UPROPERTY(ReplicatedUsing=OnRep_DesiredMesh)
	TObjectPtr<USkeletalMesh> DesiredMesh;

private:
	UFUNCTION()
	void OnRep_DesiredMesh() const;
};
