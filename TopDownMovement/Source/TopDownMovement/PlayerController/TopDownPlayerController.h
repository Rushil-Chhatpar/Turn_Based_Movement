// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "TopDownPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNMOVEMENT_API ATopDownPlayerController : public APlayerController
{
	GENERATED_BODY()

	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;

	void LeftClick(const FInputActionValue& Value);

	void RightClick(const FInputActionValue& Value);

public:
    virtual void SetupInputComponent() override;

	UPROPERTY(EditAnywhere, Category = "Input")
		class UInputMappingContext* MappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
		class UInputAction* LeftClickAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
		class UInputAction* RightClickAction;

	UFUNCTION(Server, Reliable)
		void Server_SpawnPlayer();

public:
	virtual void Tick(float DeltaSeconds) override;

};
