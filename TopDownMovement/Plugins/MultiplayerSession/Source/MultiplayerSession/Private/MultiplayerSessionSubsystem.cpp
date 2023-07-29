// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerSessionSubsystem.h"

#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"

UMultiplayerSessionSubsystem::UMultiplayerSessionSubsystem()
    : CreateSessionCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateSessionComplete))
    , FindSessionsCompleteDelegate(FOnFindSessionsCompleteDelegate::CreateUObject(this, &ThisClass::OnFindSessionComplete))
    , JoinSessionCompleteDelegate(FOnJoinSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnJoinSessionComplete))
    , DestroySessionCompleteDelegate(FOnDestroySessionCompleteDelegate::CreateUObject(this, &ThisClass::OnDestroySessionComplete))
    , StartSessionCompleteDelegate(FOnStartSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnStartSessionComplete))
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (Subsystem != nullptr)
    {
        SessionInterface = Subsystem->GetSessionInterface();
    }
}

void UMultiplayerSessionSubsystem::CreateSession(int32 NumPublicConnections)
{
    if (!SessionInterface.IsValid())
        return;

    //
    // Destroy an already existing session
    auto ExistingSession = SessionInterface->GetNamedSession(NAME_GameSession);
    if(ExistingSession)
    {
        bCreateSessionOnDestroy = true;
        LastNumPublicConnections = NumPublicConnections;
        DestroySession();
    }

    //
    // Assign and store the delegate handle for creating sessions
    CreateSessionCompleteDelegateHandle = SessionInterface->AddOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegate);

    LastSessionSettings = MakeShareable(new FOnlineSessionSettings());

    if (IOnlineSubsystem::Get()->GetSubsystemName() == FName("NULL"))
        LastSessionSettings->bIsLANMatch = true;
    else if (IOnlineSubsystem::Get()->GetSubsystemName() == FName("Steam"))
        LastSessionSettings->bIsLANMatch = false;

    LastSessionSettings->NumPublicConnections = NumPublicConnections;
    LastSessionSettings->bAllowJoinInProgress = true;
    LastSessionSettings->bAllowJoinViaPresence = true;
    LastSessionSettings->bShouldAdvertise = true;
    LastSessionSettings->bUsesPresence = true;
    LastSessionSettings->bUseLobbiesIfAvailable = true;
    LastSessionSettings->BuildUniqueId = 1;
    //
    // This is for when you want to pass in a value to be accessed by session settings anywhere using the Get() function in session settings
    // LastSessionSettings->Set(FName("KeyName"), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing, someValue);
    //

    const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();

    // Create a session
    if (!SessionInterface->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, *LastSessionSettings))
    {
        // If the session creation failed, clear the subsystem delegate handle
        SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);

        // Broadcasting custom delegate
        MultiplayerOnCreateSessionComplete.Broadcast(false);
    }
    
}

void UMultiplayerSessionSubsystem::FindSession(int32 MaxSearchResults)
{
    if (!SessionInterface.IsValid())
        return;

    FindSessionsCompleteDelegateHandle = SessionInterface->AddOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegate);
    LastSessionSearch = MakeShareable(new FOnlineSessionSearch);

    // Search settings
    LastSessionSearch->MaxSearchResults = MaxSearchResults;
    if (IOnlineSubsystem::Get()->GetSubsystemName() == FName("NULL"))
        LastSessionSearch->bIsLanQuery = true;
    else if (IOnlineSubsystem::Get()->GetSubsystemName() == FName("Steam"))
        LastSessionSearch->bIsLanQuery = false;
    LastSessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

    const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
    if (!SessionInterface->FindSessions(*LocalPlayer->GetPreferredUniqueNetId(), LastSessionSearch.ToSharedRef()))
    {
        // If the session find was failed, clear the subsystem delegate handle
        SessionInterface->ClearOnCancelFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);

        // Broadcasting custom delegate
        MultiplayerOnFindSessionComplete.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
    }
}

void UMultiplayerSessionSubsystem::JoinSession(FOnlineSessionSearchResult& SessionResult)
{
    if (!SessionInterface.IsValid())
    {
        MultiplayerOnJoinSessionComplete.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
        return;
    }

    JoinSessionCompleteDelegateHandle = SessionInterface->AddOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegate);

    const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();

    if (!SessionInterface->JoinSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, SessionResult))
    {
        // If the session join failed, clear the subsystem delegate handle
        SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);

        // Broadcasting custom delegate
        MultiplayerOnJoinSessionComplete.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
    }
}

void UMultiplayerSessionSubsystem::DestroySession()
{
    if (!SessionInterface.IsValid())
    {
        MultiplayerOnDestroySessionComplete.Broadcast(false);
        return;
    }

    DestroySessionCompleteDelegateHandle = SessionInterface->AddOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegate);

    if (!SessionInterface->DestroySession(NAME_GameSession))
    {
        // If the session destroy failed, clear the subsystem delegate handle
        SessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegateHandle);

        // Broadcasting custom delegate
        MultiplayerOnDestroySessionComplete.Broadcast(false);
    }

}

void UMultiplayerSessionSubsystem::StartSession()
{
    if(!SessionInterface.IsValid())
    {
        MultiplayerOnStartSessionComplete.Broadcast(false);
        return;
    }

    StartSessionCompleteDelegateHandle = SessionInterface->AddOnStartSessionCompleteDelegate_Handle(StartSessionCompleteDelegate);

    if(!SessionInterface->StartSession(NAME_GameSession))
    {
        // If the session start failed, clear the subsystem delegate handle
        SessionInterface->ClearOnStartSessionCompleteDelegate_Handle(StartSessionCompleteDelegateHandle);

        // Broadcasting custom delegate
        MultiplayerOnStartSessionComplete.Broadcast(false);
    }
}

void UMultiplayerSessionSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
}

void UMultiplayerSessionSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
    if(SessionInterface)
    {
        // Clear the on Create session delegate handle because the session was created successfully 
        SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);
    }

    // Broadcasting custom delegate
    MultiplayerOnCreateSessionComplete.Broadcast(bWasSuccessful);
}

void UMultiplayerSessionSubsystem::OnFindSessionComplete(bool bWasSuccessful)
{
    if(SessionInterface)
    {
        // Clear the on Find session delegate handle because the session find was successful
        SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);
    }

    // If the number of found sessions is zero, return 
    if (LastSessionSearch->SearchResults.Num() <= 0)
    {
        MultiplayerOnFindSessionComplete.Broadcast(TArray <FOnlineSessionSearchResult> (), false);
        return;
    }

    // Broadcasting custom delegate
    MultiplayerOnFindSessionComplete.Broadcast(LastSessionSearch->SearchResults, true);
}

void UMultiplayerSessionSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
    if(SessionInterface)
    {
        // Clear the on Join session delegate handle because the session was joined successfully
        SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);
    }

    MultiplayerOnJoinSessionComplete.Broadcast(Result);
}

void UMultiplayerSessionSubsystem::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
    if(SessionInterface)
    {
        // Clear the on Destroy session delegate handle because the session was destroyed successfully
        SessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegateHandle);
    }

    // Create a session if bCreateSessionOnDestroy
    if(bWasSuccessful && bCreateSessionOnDestroy)
    {
        bCreateSessionOnDestroy = false;
        CreateSession(LastNumPublicConnections);
    }

    // Broadcasting custom delegate
    MultiplayerOnDestroySessionComplete.Broadcast(bWasSuccessful);
}

void UMultiplayerSessionSubsystem::OnStartSessionComplete(FName SessionName, bool bWasSuccessful)
{
    if(SessionInterface)
    {
        // Clear the on Start session delegate handle because the session was started successfully
        SessionInterface->ClearOnStartSessionCompleteDelegate_Handle(StartSessionCompleteDelegateHandle);
    }

    // Broadcasting custom delegate
    MultiplayerOnStartSessionComplete.Broadcast(bWasSuccessful);
}
