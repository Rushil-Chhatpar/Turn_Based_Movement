// Fill out your copyright notice in the Description page of Project Settings.


#include "TopDownMovement/GameMode/TopDownGameMode.h"
#include "TopDownMovement/PlayerController/TopDownPlayerController.h"
#include "GameFramework/PlayerStart.h"
#include "TopDownMovement/Character/TopDownCharacter.h"
#include "Kismet/GameplayStatics.h"


void ATopDownGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    ATopDownPlayerController* PC = Cast<ATopDownPlayerController>(NewPlayer);

    if (PC != nullptr)
        PlayerControllers.Add(PC);
}

void ATopDownGameMode::Logout(AController* Exiting)
{
    Super::Logout(Exiting);

    ATopDownPlayerController* PC = Cast<ATopDownPlayerController>(Exiting);

    if (PC != nullptr)
        PlayerControllers.Remove(PC);

    
}

void ATopDownGameMode::SpawnPlayer(APlayerController* PlayerController)
{
    APawn* PlayerPawn = PlayerController->GetPawn();

    if(PlayerPawn!=nullptr)
    {
        PlayerPawn->Destroy();
    }

    TArray<AActor*> PlayerStarts;

    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);

    int rand = FMath::RandRange(0, PlayerStarts.Num() - 1);

    FActorSpawnParameters SpawnParameters;
    SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    //ATopDownCharacter* Character = GetWorld()->SpawnActor(CharacterClass, PlayerStarts[rand]->GetActorLocation(), PlayerStarts[rand]->GetActorRotation(), SpawnParameters);
    ATopDownCharacter* Character = GetWorld()->SpawnActor<ATopDownCharacter>(CharacterClass, PlayerStarts[rand]->GetActorLocation(), PlayerStarts[rand]->GetActorRotation(), SpawnParameters);

    PlayerController->Possess(Character);
}


