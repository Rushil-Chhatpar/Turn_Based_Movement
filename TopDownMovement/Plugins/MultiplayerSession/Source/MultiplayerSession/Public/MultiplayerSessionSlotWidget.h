// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSessionSettings.h"
#include "MultiplayerSessionSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSION_API UMultiplayerSessionSlotWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	void MenuSetup(FOnlineSessionSearchResult Result, FString ServerName);

protected:
	virtual bool Initialize() override;
	virtual void NativeDestruct() override;

private:
	// Subsystem for handling online multiplayer sessions
	class UMultiplayerSessionSubsystem* MultiplayerSessionSubsystem;

	FOnlineSessionSearchResult SearchResult;

	//
	// UI elements
	//

	UPROPERTY(meta = (BindWidget))
		class UButton* JoinServerButton;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TB_ServerName;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TB_Players;

private:
	//
	// Button callbacks
	//

	UFUNCTION()
		void JoinServerButtonClicked();
};
