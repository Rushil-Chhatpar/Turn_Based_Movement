// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Dummy.generated.h"

UCLASS()
class TOPDOWNMOVEMENT_API ADummy : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ADummy();

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		class UStaticMeshComponent* StaticMesh;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Box Collider")
		class UBoxComponent* BoxCollider;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Movement")
		class UFloatingPawnMovement* PawnMovement;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Team")
		int TeamID;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveTo(FVector Location);

	UFUNCTION(Server, Reliable)
		void Server_MoveTo(FVector Location);

};
