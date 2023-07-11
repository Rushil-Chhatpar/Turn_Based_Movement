// Fill out your copyright notice in the Description page of Project Settings.


#include "TopDownMovement/Dummies/DummyMk3.h"
#include "Net/UnrealNetwork.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Components/TextRenderComponent.h"

// Sets default values
ADummyMk3::ADummyMk3()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh");
	StaticMesh->SetCollisionProfileName("NoCollision");
	StaticMesh->SetupAttachment(RootComponent);

	TextRender = CreateDefaultSubobject<UTextRenderComponent>("Text");
	TextRender->SetupAttachment(RootComponent);

	this->Tags.Empty();
	this->Tags.Add("DummyMk3");

	SetReplicates(true);
	SetReplicateMovement(true);
}

// Called when the game starts or when spawned
void ADummyMk3::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADummyMk3::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AActor* test = Owner;
	ENetRole role = GetLocalRole();
    int bp = 0;


}

void ADummyMk3::MoveDummy(FVector Location)
{
	//FVector loc = GetActorLocation();
	//loc.Z += 50.0f;
	//SetActorLocation(loc);
	//Server_MoveDummy(Location);
	ENetRole role = GetLocalRole();
	int bp = 0;
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), Location);
}

void ADummyMk3::SetAutoRole()
{
	SetAutonomousProxy(true);
}

void ADummyMk3::Server_MoveDummy_Implementation(FVector Location)
{
	FVector loc = GetActorLocation();
	loc.Z += 50.0f;
	SetActorLocation(loc);
}


