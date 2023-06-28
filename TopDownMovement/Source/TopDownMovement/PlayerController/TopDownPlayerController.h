// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TopDownPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNMOVEMENT_API ATopDownPlayerController : public APlayerController
{
	GENERATED_BODY()

	virtual void OnPossess(APawn* InPawn) override;

public:
	virtual void Tick(float DeltaSeconds) override;

};
