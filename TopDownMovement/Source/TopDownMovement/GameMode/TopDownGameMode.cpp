// Fill out your copyright notice in the Description page of Project Settings.


#include "TopDownMovement/GameMode/TopDownGameMode.h"
#include "TopDownMovement/PlayerController/TopDownPlayerController.h"
#include "GameFramework/PlayerStart.h"
#include "TopDownMovement/Character/TopDownCharacter.h"
#include "TopDownMovement/Dummies/DummyMk3.h"
#include "Kismet/GameplayStatics.h"
#include "TopDownMovement/DummySpawnLocation.h"

void ATopDownGameMode::BeginPlay()
{
    Super::BeginPlay();
    //TArray<AActor*> Locations;
    //FActorSpawnParameters SpawnParameters;
    //SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    //UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADummySpawnLocation::StaticClass(), Locations);
    //for (int i = 0; i < Locations.Num(); i++)
    //{
    //    GetWorld()->SpawnActor<ADummy>(DummyClass, Locations[i]->GetActorLocation(), Locations[i]->GetActorRotation(), SpawnParameters);
    //}
    Server_SpawnDummies();
}

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
    ATopDownCharacter* Character = GetWorld()->SpawnActor<ATopDownCharacter>(CharacterClass, PlayerStarts[index]->GetActorLocation(), PlayerStarts[index]->GetActorRotation(), SpawnParameters);
    index++;

    PlayerController->Possess(Character);
}

void ATopDownGameMode::Server_SpawnDummies_Implementation()
{
    TArray<AActor*> Locations;
    FActorSpawnParameters SpawnParameters;
    SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADummySpawnLocation::StaticClass(), Locations);
    for (int i = 0; i < Locations.Num(); i++)
    {
        GetWorld()->SpawnActor<ADummyMk3>(DummyClass, Locations[i]->GetActorLocation(), Locations[i]->GetActorRotation(), SpawnParameters);
    }
}


