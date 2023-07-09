// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "TopDownCharacter.generated.h"

UCLASS()
class TOPDOWNMOVEMENT_API ATopDownCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATopDownCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		class USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, Category = "Input")
		class UInputMappingContext* MappingContext;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	void LeftClick(const FInputActionValue& Value);

	void RightClick(const FInputActionValue& Value);

	UPROPERTY(VisibleAnywhere)
		class ADummyMk3* CurrentSelection;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(Server, Reliable)
		void Server_MoveDummyTo(FVector Location);

	UPROPERTY(EditAnywhere, Category = "Dummy Class")
		TSubclassOf<class ADummyMk3> DummyClass;

	UFUNCTION(Server, Reliable)
		void Server_SpawnDummies();
	// Called to bind functionality to input
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	UFUNCTION(Server, Reliable)
		void Server_SpawnDummy(FVector Location);

	class ADummyMk3* TempDummy = nullptr;
};
