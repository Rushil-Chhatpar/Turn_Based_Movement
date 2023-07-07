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


	virtual void BeginPlay() override;
public:

	virtual void PostLogin(APlayerController* NewPlayer) override;
	
	virtual void Logout(AController* Exiting) override;


	void SpawnPlayer(APlayerController* PlayerController);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Pawn")
		TSubclassOf<class ATopDownCharacter> CharacterClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dummy")
		TSubclassOf<class ADummy> DummyClass;

	UFUNCTION(Server, Reliable)
		void Server_SpawnDummies();

protected:
	int index = 0;
	TArray<class ATopDownPlayerController*> PlayerControllers;
};
