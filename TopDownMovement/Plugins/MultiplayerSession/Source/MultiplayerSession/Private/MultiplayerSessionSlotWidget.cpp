// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerSessionSlotWidget.h"

#include "MultiplayerSessionSubsystem.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UMultiplayerSessionSlotWidget::MenuSetup(FOnlineSessionSearchResult Result, FString ServerName)
{
    SearchResult = Result;
    TB_ServerName->SetText(FText::FromString(ServerName));

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
