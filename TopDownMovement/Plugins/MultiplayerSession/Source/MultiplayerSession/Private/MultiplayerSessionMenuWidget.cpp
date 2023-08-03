// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerSessionMenuWidget.h"

#include "MultiplayerSessionSlotWidget.h"
#include "Components/Button.h"
#include "MultiplayerSessionSubsystem.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "Components/EditableText.h"
#include "Components/WidgetSwitcher.h"
#include "Components/ScrollBox.h"
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
            FInputModeGameAndUI InputMode;
            InputMode.SetWidgetToFocus(TakeWidget());
            InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
            InputModeData.SetWidgetToFocus(TakeWidget());
            InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

            // Set input mode data for the player controller
            PlayerController->SetInputMode(InputMode);

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
    if (ServerBackButton)
        ServerBackButton->OnClicked.AddDynamic(this, &ThisClass::ServerBackButtonClicked);
    if (JoinBackButton)
        JoinBackButton->OnClicked.AddDynamic(this, &ThisClass::JoinBackButtonClicked);
    if (CreateServerButton)
        CreateServerButton->OnClicked.AddDynamic(this, &ThisClass::CreateServerButtonClicked);

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
        UMultiplayerSessionSlotWidget* ServerSlot = CreateWidget<UMultiplayerSessionSlotWidget>(this->GetGameInstance(), ServerSlotClass);
        //UMultiplayerSessionSlotWidget* ServerSlot = Cast<UMultiplayerSessionSlotWidget>(SlotWidget);
        if (ServerSlot)
        {
            FString ServerName;
            SearchResult.Session.SessionSettings.Get("SERVER_NAME_KEY", ServerName);

            ServerSlot->MenuSetup(SearchResult, ServerName);
            ServerlListBox->AddChild(ServerSlot);
        }
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
    WidgetSwitcher->SetActiveWidgetIndex(1);

    //if(MultiplayerSessionSubsystem)
    //{
    //    MultiplayerSessionSubsystem->CreateSession(4);
    //}
}

void UMultiplayerSessionMenuWidget::JoinButtonClicked()
{
    WidgetSwitcher->SetActiveWidgetIndex(2);
    if(MultiplayerSessionSubsystem)
    {
        MultiplayerSessionSubsystem->FindSession(10000);
    }
}

void UMultiplayerSessionMenuWidget::ServerBackButtonClicked()
{
    WidgetSwitcher->SetActiveWidgetIndex(0);
}

void UMultiplayerSessionMenuWidget::JoinBackButtonClicked()
{
    WidgetSwitcher->SetActiveWidgetIndex(0);
}

void UMultiplayerSessionMenuWidget::CreateServerButtonClicked()
{
    FString ServerName = ServerNameEditableText->GetText().ToString();
    if (MultiplayerSessionSubsystem)
    {
        MultiplayerSessionSubsystem->CreateSession(2, ServerName);
    }
    
    
}
