// Fill out your copyright notice in the Description page of Project Settings.


#include "TopDownMovement/GameInstance/TopDownGameInstance.h"
#include "Engine/World.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Kismet/GameplayStatics.h"

UTopDownGameInstance::UTopDownGameInstance()
{
    MySessionName = FName("My Session");
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
            SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UTopDownGameInstance::OnFindSessionComplete);
            SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UTopDownGameInstance::OnJoinSessionComplete);
        }
    }

}

void UTopDownGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
    UE_LOG(LogTemp, Warning, TEXT("Success: %d"), bWasSuccessful);

    if(bWasSuccessful)
    {
        GetWorld()->ServerTravel("/Game/Levels/TestingLevel?listen");
    }
}

void UTopDownGameInstance::OnFindSessionComplete(bool bWasSuccessful)
{
    SearchingForServer.Broadcast(false);
    UE_LOG(LogTemp, Warning, TEXT("Find Success: %d"), bWasSuccessful);
    if(bWasSuccessful)
    {
        int32 ArrayIndex = -1;

        TArray<FOnlineSessionSearchResult> SearchResults = SessionSearch->SearchResults;

        for(FOnlineSessionSearchResult Result : SearchResults)
        {
            ++ArrayIndex;
            if(!Result.IsValid())
                continue;

            FString ServerName = "Empty Server Name";
            FString HostName = "Empty Host Name";

            Result.Session.SessionSettings.Get(FName("SERVER_NAME_KEY"), ServerName);
            Result.Session.SessionSettings.Get(FName("SERVER_HOSTNAME_KEY"), HostName);

            FServerInfo Info;
            Info.ServerName = ServerName;
            Info.MaxPlayers = Result.Session.SessionSettings.NumPublicConnections;
            Info.CurrentPlayers = Info.MaxPlayers - Result.Session.NumOpenPublicConnections;
            Info.SetPlayerCount();
            Info.ServerArrayIndex = ArrayIndex;
            ServerListDelegate.Broadcast(Info);
            
        }
        //if (SearchResults.Num() > 0)
        //{
        //    SessionInterface->JoinSession(0, "MySession", SearchResults[0]);
        //}

        UE_LOG(LogTemp, Warning, TEXT("Find Count: %d"), SearchResults.Num());
    }
}

void UTopDownGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    //ANOTHER WAY OF DOING IT
    //APlayerController* PlayerController = GetFirstLocalPlayerController(GetWorld());
    if(PlayerController!=nullptr)
    {
        FString JoinAddress = "";
        SessionInterface->GetResolvedConnectString(SessionName, JoinAddress);
        if (JoinAddress != "")
            PlayerController->ClientTravel(JoinAddress, TRAVEL_Absolute);
    }
}

void UTopDownGameInstance::CreateServer(FString ServerName, FString HostName)
{

    UE_LOG(LogTemp, Warning, TEXT("Create server"));
    FOnlineSessionSettings SessionSettings;
    SessionSettings.bAllowJoinInProgress = true;
    SessionSettings.bIsDedicated = false;
    SessionSettings.bIsLANMatch = false;
    SessionSettings.bShouldAdvertise = true;
    SessionSettings.bUsesPresence = true;
    SessionSettings.NumPublicConnections = 4;
    SessionSettings.bUseLobbiesIfAvailable = true;
    SessionSettings.Set(FName("SERVER_NAME_KEY"), ServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
    SessionSettings.Set(FName("SERVER_HOSTNAME_KEY"), HostName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);


    SessionInterface->CreateSession(0, MySessionName, SessionSettings);
}

void UTopDownGameInstance::FindServer()
{
    SessionSearch = MakeShareable(new FOnlineSessionSearch());
    SessionSearch->bIsLanQuery = false;
    SessionSearch->MaxSearchResults = 10000;
    SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

    SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());

    SearchingForServer.Broadcast(true);
}

void UTopDownGameInstance::JoinServer(int32 ArrayIndex)
{
    FOnlineSessionSearchResult Result = SessionSearch->SearchResults[ArrayIndex];
    if(Result.IsValid())
    {
        SessionInterface->JoinSession(0, MySessionName, Result);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to join server at index %d"), ArrayIndex);
    }
}
