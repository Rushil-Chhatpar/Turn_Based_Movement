// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MultiplayerSessionMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSION_API UMultiplayerSessionMenuWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
		void MenuSetup();

protected:
	virtual bool Initialize() override;
	virtual void NativeDestruct() override;

	//
	// Callbacks for the custom delegates on Multiplayer Session Subsystem
	//
	UFUNCTION()
		void OnCreateSession(bool bWasSuccessful);

	void OnFindSession(const TArray<FOnlineSessionSearchResult>& SearchResults, bool bWasSuccessful);
	void OnJoinSession(EOnJoinSessionCompleteResult::Type Result);

	UFUNCTION()
		void OnDestroySession(bool bWasSuccessful);
	UFUNCTION()
		void OnStartSession(bool bWasSuccessful);

	///////////////////

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Server Slot Widget")
		TSubclassOf<class UMultiplayerSessionSlotWidget> ServerSlotClass;

private:
	// Subsystem for handling online multiplayer sessions
	class UMultiplayerSessionSubsystem* MultiplayerSessionSubsystem;

	FString PathToLobby{ TEXT("") };

	//
	// UI elements
	//

	UPROPERTY(meta = (BindWidget))
		class UButton* HostButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* JoinButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* ServerBackButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* JoinBackButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* CreateServerButton;

	UPROPERTY(meta = (BindWidget))
		class UWidgetSwitcher* WidgetSwitcher;

	UPROPERTY(meta = (BindWidget))
		class UEditableText* ServerNameEditableText;

	UPROPERTY(meta = (BindWidget))
		class UScrollBox* ServerlListBox;

private:

	void MenuTearDown();

	//
	// Button Callbacks
	//

	UFUNCTION()
		void HostButtonClicked();
	UFUNCTION()
		void JoinButtonClicked();
	UFUNCTION()
		void ServerBackButtonClicked();
	UFUNCTION()
		void JoinBackButtonClicked();
	UFUNCTION()
		void CreateServerButtonClicked();
};
