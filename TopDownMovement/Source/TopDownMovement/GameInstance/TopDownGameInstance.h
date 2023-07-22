// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "Interfaces/OnlineSessionInterface.h"

#include "TopDownGameInstance.generated.h"


USTRUCT(BlueprintType)
struct FServerInfo
{
    GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly)
		FString ServerName;

	UPROPERTY(BlueprintReadOnly)
		int32 CurrentPlayers;

	UPROPERTY(BlueprintReadOnly)
		int32 MaxPlayers;



};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FServerDelegate, FServerInfo, ServerListDelegate);

/**
 * 
 */
UCLASS()
class TOPDOWNMOVEMENT_API UTopDownGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UTopDownGameInstance();

protected:
	UPROPERTY(BlueprintAssignable)
		FServerDelegate ServerListDelegate;

protected:
	virtual void Init() override;
	
	virtual void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	virtual void OnFindSessionComplete(bool bWasSuccessful);
	virtual void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	UFUNCTION(BlueprintCallable)
		void CreateServer(FString ServerName, FString HostName);

	UFUNCTION(BlueprintCallable)
		void JoinServer();

protected:
	IOnlineSessionPtr SessionInterface;

	TSharedPtr<FOnlineSessionSearch> SessionSearch;
};
