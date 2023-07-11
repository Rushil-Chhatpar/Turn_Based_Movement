// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DummyMk3.generated.h"

UCLASS()
class TOPDOWNMOVEMENT_API ADummyMk3 : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADummyMk3();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Mesh")
		class UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Text")
		class UTextRenderComponent* TextRender;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void MoveDummy(FVector Location);

	UFUNCTION(Server, Reliable)
		void Server_MoveDummy(FVector Location);

	void SetAutoRole();

	
};
