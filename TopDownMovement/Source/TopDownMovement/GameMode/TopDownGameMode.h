// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "TopDownGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNMOVEMENT_API ATopDownGameMode : public AGameMode
{
	GENERATED_BODY()


public:

	virtual void PostLogin(APlayerController* NewPlayer) override;
	
	virtual void Logout(AController* Exiting) override;


	void SpawnPlayer(APlayerController* PlayerController);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Pawn")
		TSubclassOf<class ATopDownCharacter> CharacterClass;

protected:

	TArray<class ATopDownPlayerController*> PlayerControllers;
};
