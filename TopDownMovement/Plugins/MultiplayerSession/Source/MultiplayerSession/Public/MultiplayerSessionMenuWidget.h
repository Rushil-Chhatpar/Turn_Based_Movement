// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
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
private:
	// Subsystem for handling online multiplayer sessions
	class UMultiplayerSessionSubsystem* MultiplayerSessionSubsystem;

	//
	//Buttons
	//

	UPROPERTY(meta = (BindWidget))
		class UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* JoinButton;

private:

	void MenuTearDown();

	//
	// Button Callbacks
	//

	UFUNCTION()
		void HostButtonClicked();

	UFUNCTION()
		void JoinButtonClicked();
};
