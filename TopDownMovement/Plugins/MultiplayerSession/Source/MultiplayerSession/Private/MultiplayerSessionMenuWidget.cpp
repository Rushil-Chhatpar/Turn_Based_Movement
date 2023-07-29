// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerSessionMenuWidget.h"
#include "Components/Button.h"
#include "MultiplayerSessionSubsystem.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "GameFramework/GameModeBase.h"


void UMultiplayerSessionMenuWidget::MenuSetup()
{
    AddToViewport();
    SetVisibility(ESlateVisibility::Visible);
    bIsFocusable = true;

    UWorld* World = GetWorld();

    if (World)
    {
        APlayerController* PlayerController = World->GetFirstPlayerController();
        if (PlayerController)
        {
            // Create and set input mode data for the menu widget
            FInputModeUIOnly InputModeData;
            InputModeData.SetWidgetToFocus(TakeWidget());
            InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

            // Set input mode data for the player controller
            PlayerController->SetInputMode(InputModeData);

            PlayerController->SetShowMouseCursor(true);
        }
    }

    UGameInstance* GameInstance = GetGameInstance();
    if (GameInstance)
    {
        MultiplayerSessionSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionSubsystem>();
    }

    if (MultiplayerSessionSubsystem)
    {
        MultiplayerSessionSubsystem->MultiplayerOnCreateSessionComplete.AddDynamic(this, &ThisClass::OnCreateSession);
        MultiplayerSessionSubsystem->MultiplayerOnFindSessionComplete.AddUObject(this, &ThisClass::OnFindSession);
        MultiplayerSessionSubsystem->MultiplayerOnJoinSessionComplete.AddUObject(this, &ThisClass::OnJoinSession);
        MultiplayerSessionSubsystem->MultiplayerOnDestroySessionComplete.AddDynamic(this, &ThisClass::OnDestroySession);
        MultiplayerSessionSubsystem->MultiplayerOnStartSessionComplete.AddDynamic(this, &ThisClass::OnStartSession);
    }
}

bool UMultiplayerSessionMenuWidget::Initialize()
{
    if (!Super::Initialize())
        return false;

    if(HostButton)
        HostButton->OnClicked.AddDynamic(this, &ThisClass::HostButtonClicked);
    if (JoinButton)
        JoinButton->OnClicked.AddDynamic(this, &ThisClass::JoinButtonClicked);

    return true;
}

void UMultiplayerSessionMenuWidget::NativeDestruct()
{
    MenuTearDown();
    Super::NativeDestruct();
}

void UMultiplayerSessionMenuWidget::OnCreateSession(bool bWasSuccessful)
{
    if(bWasSuccessful)
    {
        if (GEngine)
            GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, FString(TEXT("Session Created")));
    }
    GetWorld()->ServerTravel("/Game/Levels/TestingLevel?listen");
}

void UMultiplayerSessionMenuWidget::OnFindSession(const TArray<FOnlineSessionSearchResult>& SearchResults, bool bWasSuccessful)
{
    if (!bWasSuccessful)
        return;

    for (auto SearchResult : SearchResults)
    {
        
    }
}

void UMultiplayerSessionMenuWidget::OnJoinSession(EOnJoinSessionCompleteResult::Type Result)
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();

    if (Subsystem)
    {
        IOnlineSessionPtr SessionInterface = Subsystem->GetSessionInterface();

        if (SessionInterface.IsValid())
        {
            // Get the session address
            FString Address;
            SessionInterface->GetResolvedConnectString(NAME_GameSession, Address);
            
            // Client travel
            APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController(GetWorld());
            PlayerController->ClientTravel(Address, TRAVEL_Absolute);
        }
    }
}

void UMultiplayerSessionMenuWidget::OnDestroySession(bool bWasSuccessful)
{
    UWorld* World = GetWorld();
    if(World)
    {
        AGameModeBase* GameMode = World->GetAuthGameMode();
        // if on server
        if(GameMode)
        {
            GameMode->ReturnToMainMenuHost();
        }
        // if on client
        else
        {
            APlayerController* PlayerController = World->GetFirstPlayerController();
            if(PlayerController)
            {
                PlayerController->ClientReturnToMainMenuWithTextReason(FText());
            }
        }
    }
}

void UMultiplayerSessionMenuWidget::OnStartSession(bool bWasSuccessful)
{
}

void UMultiplayerSessionMenuWidget::MenuTearDown()
{
    RemoveFromParent();
    
}

void UMultiplayerSessionMenuWidget::HostButtonClicked()
{
    if(MultiplayerSessionSubsystem)
    {
        MultiplayerSessionSubsystem->CreateSession(4);
    }
}

void UMultiplayerSessionMenuWidget::JoinButtonClicked()
{
    if(MultiplayerSessionSubsystem)
    {
        MultiplayerSessionSubsystem->FindSession(10000);
    }
}
