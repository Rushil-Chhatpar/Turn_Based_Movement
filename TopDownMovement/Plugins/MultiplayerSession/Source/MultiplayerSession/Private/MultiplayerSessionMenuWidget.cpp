// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerSessionMenuWidget.h"
#include "Components/Button.h"
#include "MultiplayerSessionSubsystem.h"


void UMultiplayerSessionMenuWidget::MenuSetup()
{
    AddToViewport();
    SetVisibility(ESlateVisibility::Visible);
    bIsFocusable = true;

    UWorld* World = GetWorld();

    if(World)
    {
        APlayerController* PlayerController = World->GetFirstPlayerController();
        if(PlayerController)
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
    if(GameInstance)
    {
        MultiplayerSessionSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionSubsystem>();
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
    if (GEngine)
        GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Magenta, FString(TEXT("Join")));
}
