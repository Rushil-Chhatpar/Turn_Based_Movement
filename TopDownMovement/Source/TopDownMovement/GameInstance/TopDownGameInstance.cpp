// Fill out your copyright notice in the Description page of Project Settings.


#include "TopDownMovement/GameInstance/TopDownGameInstance.h"

#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"

UTopDownGameInstance::UTopDownGameInstance()
{

}

void UTopDownGameInstance::Init()
{
    Super::Init();

    if(IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get())
    {
        SessionInterface = Subsystem->GetSessionInterface();
        if(SessionInterface.IsValid())
        {
            //Bind delegates
            SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UTopDownGameInstance::OnCreateSessionComplete);

        }
    }

}

void UTopDownGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
    UE_LOG(LogTemp, Warning, TEXT("Success: %d"), bWasSuccessful);
}

void UTopDownGameInstance::CreateServer()
{

    UE_LOG(LogTemp, Warning, TEXT("Create server"));
    FOnlineSessionSettings SessionSettings;
    SessionSettings.bAllowJoinInProgress = true;
    SessionSettings.bIsDedicated = true;
    SessionSettings.bShouldAdvertise = true;
    SessionSettings.bUsesPresence = true;
    SessionSettings.NumPublicConnections = 4;


    SessionInterface->CreateSession(0, FName("My Session"), SessionSettings);
}

void UTopDownGameInstance::JoinServer()
{
}
