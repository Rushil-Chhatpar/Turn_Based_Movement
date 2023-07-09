// Fill out your copyright notice in the Description page of Project Settings.


#include "TopDownMovement/PlayerController/TopDownPlayerController.h"
#include "TopDownMovement/GameMode/TopDownGameMode.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "TopDownMovement/Character/TopDownCharacter.h"

void ATopDownPlayerController::BeginPlay()
{
    Super::BeginPlay();


    if(this->IsLocalController() == true)
    {
        Server_SpawnPlayer();
    }

    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        Subsystem->AddMappingContext(MappingContext, 0);
    }
}

void ATopDownPlayerController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    bShowMouseCursor = true;
    bEnableClickEvents = true;
}

void ATopDownPlayerController::LeftClick(const FInputActionValue& Value)
{
    ATopDownCharacter* PCharacter = Cast<ATopDownCharacter>(this->GetPawn());
    PCharacter->LeftClick(Value);
}

void ATopDownPlayerController::RightClick(const FInputActionValue& Value)
{
    ATopDownCharacter* PCharacter = Cast<ATopDownCharacter>(this->GetPawn());
    PCharacter->RightClick(Value);
}

void ATopDownPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    UEnhancedInputComponent* EnhancedInput = CastChecked<UEnhancedInputComponent>(InputComponent);
    if (EnhancedInput != nullptr)
    {
        EnhancedInput->BindAction(LeftClickAction, ETriggerEvent::Completed, this, &ATopDownPlayerController::LeftClick);

        EnhancedInput->BindAction(RightClickAction, ETriggerEvent::Completed, this, &ATopDownPlayerController::RightClick);
    }
}

void ATopDownPlayerController::Server_SpawnPlayer_Implementation()
{
    ATopDownGameMode* GM = Cast<ATopDownGameMode>(GetWorld()->GetAuthGameMode());
    GM->SpawnPlayer(this);
}

void ATopDownPlayerController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}
