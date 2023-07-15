// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "Interfaces/OnlineSessionInterface.h"

#include "TopDownGameInstance.generated.h"

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
	virtual void Init() override;
	
	virtual void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	UFUNCTION(BlueprintCallable)
		void CreateServer();

	UFUNCTION(BlueprintCallable)
		void JoinServer();

protected:
	IOnlineSessionPtr SessionInterface;
};
