// Fill out your copyright notice in the Description page of Project Settings.


#include "TopDownMovement/PlayerController/TopDownPlayerController.h"

void ATopDownPlayerController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    bShowMouseCursor = true;
    bEnableClickEvents = true;
}

void ATopDownPlayerController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}
