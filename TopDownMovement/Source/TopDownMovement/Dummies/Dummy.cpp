// Fill out your copyright notice in the Description page of Project Settings.


#include "TopDownMovement/Dummies/Dummy.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Net/UnrealNetwork.h"


// Sets default values
ADummy::ADummy()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollider = CreateDefaultSubobject<UBoxComponent>("Box Collider");
	SetRootComponent(BoxCollider);
	BoxCollider->SetCollisionProfileName("BlockAllDynamic");





	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh");
	StaticMesh->SetupAttachment(BoxCollider);

	PawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>("Pawn Movement");
	PawnMovement->UpdatedComponent = RootComponent;

	Tags.Empty();
	Tags.Add("Dummy");

	TeamID = 0;

	SetReplicates(true);
	SetReplicateMovement(true);
}

// Called when the game starts or when spawned
void ADummy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADummy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ADummy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
}

void ADummy::MoveTo(FVector Location)
{
	//AController* c = GetController();
	//UAIBlueprintHelperLibrary::SimpleMoveToLocation(c, Location);

	//ENetRole role = GetLocalRole();
	//
	//if (role <= ROLE_Authority)
		Server_MoveTo(Location);
}

void ADummy::Server_MoveTo_Implementation(FVector Location)
{
	ENetRole role = GetLocalRole();
	AController* c = GetController();
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(c, Location);
}

void ADummy::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADummy, TeamID);
	DOREPLIFETIME(ADummy, StaticMesh);
	DOREPLIFETIME(ADummy, BoxCollider);
	DOREPLIFETIME(ADummy, PawnMovement);


}
