// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "TopDownPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNMOVEMENT_API ATopDownPlayerState : public APlayerState
{
	GENERATED_BODY()

public:


    UPROPERTY(Replicated, EditAnywhere, Category = "Stats")
		int PlayerTeamID;


};
