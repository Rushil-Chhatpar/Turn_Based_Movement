// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerSessionSlotWidget.h"

#include "MultiplayerSessionSubsystem.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UMultiplayerSessionSlotWidget::MenuSetup(FOnlineSessionSearchResult Result, FString ServerName)
{
    SearchResult = Result;
    TB_ServerName->SetText(FText::FromString(ServerName));

    UWorld* World = GetWorld();

    if (World)
    {
        APlayerController* PlayerController = World->GetFirstPlayerController();
        if (PlayerController)
        {
            // Create and set input mode data for the menu widget
            FInputModeGameAndUI InputModeData;
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
}

bool UMultiplayerSessionSlotWidget::Initialize()
{
    if (!Super::Initialize())
        return false;

    if (JoinServerButton)
        JoinServerButton->OnClicked.AddDynamic(this, &ThisClass::JoinServerButtonClicked);

    return true;
}

void UMultiplayerSessionSlotWidget::NativeDestruct()
{
    Super::NativeDestruct();
}

void UMultiplayerSessionSlotWidget::JoinServerButtonClicked()
{
    if (MultiplayerSessionSubsystem)
        MultiplayerSessionSubsystem->JoinSession(SearchResult);
}
