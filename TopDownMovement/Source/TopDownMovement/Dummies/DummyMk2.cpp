// Fill out your copyright notice in the Description page of Project Settings.


#include "TopDownMovement/Dummies/DummyMk2.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/BoxComponent.h"

// Sets default values
ADummyMk2::ADummyMk2()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	BoxCollider = CreateDefaultSubobject<UBoxComponent>("Box Collider");
	SetRootComponent(BoxCollider);
	BoxCollider->SetCollisionProfileName("BlockAllDynamic");





	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh");
	StaticMesh->SetupAttachment(BoxCollider);

	PawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>("Pawn Movement");
	PawnMovement->UpdatedComponent = RootComponent;

	this->Tags.Empty();
	this->Tags.Add("DummyMk2");

	SetReplicates(true);
	SetReplicatingMovement(true);
}

// Called when the game starts or when spawned
void ADummyMk2::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADummyMk2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADummyMk2::Move(FVector Location)
{
	Server_Move(Location);
}

void ADummyMk2::Server_Move_Implementation(FVector Location)
{
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), Location);
}


void ADummyMk2::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ADummyMk2, TeamID);
	DOREPLIFETIME(ADummyMk2, StaticMesh);
	DOREPLIFETIME(ADummyMk2, BoxCollider);
	DOREPLIFETIME(ADummyMk2, PawnMovement);
}
