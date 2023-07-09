// Fill out your copyright notice in the Description page of Project Settings.


#include "TopDownMovement/Character/TopDownCharacter.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "TopDownMovement/PlayerController/TopDownPlayerController.h"
#include "TopDownMovement/Dummies/Dummy.h"
#include "TopDownMovement/Dummies/DummyMk2.h"
#include "TopDownMovement/Dummies/DummyMk3.h"
#include "Kismet/GameplayStatics.h"
#include "TopDownMovement/DummySpawnLocation.h"


// Sets default values
ATopDownCharacter::ATopDownCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("Spring Arm");
	SpringArm->bEnableCameraLag = false;
	SpringArm->SetupAttachment(RootComponent);


	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->ProjectionMode = ECameraProjectionMode::Perspective;
	Camera->SetupAttachment(SpringArm);

	CurrentSelection = nullptr;

	GetCapsuleComponent()->SetEnableGravity(false);

	SetReplicates(true);
	SetReplicateMovement(true);
}

// Called when the game starts or when spawned
void ATopDownCharacter::BeginPlay()
{
	Super::BeginPlay();

	//if(ATopDownPlayerController* TDController = Cast<ATopDownPlayerController>(GetController()))
	//{
	//    if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(TDController->GetLocalPlayer()))
	//    {
	//		Subsystem->AddMappingContext(MappingContext, 0);
	//    }
	//}
	//if (GetLocalRole() == ROLE_Authority)
	//	Server_SpawnDummies();

}

void ATopDownCharacter::LeftClick(const FInputActionValue& Value)
{
	FHitResult Hit;



	ATopDownPlayerController* PController = Cast<ATopDownPlayerController>(GetController());

	if (PController == nullptr)
		return;

	PController->GetHitResultUnderCursorByChannel(TraceTypeQuery1, false, Hit);

	AActor* hitActor = Hit.GetActor();
	FVector loc = Hit.Location;
	if(hitActor!= nullptr && hitActor->ActorHasTag("DummyMk3"))
	{
		AActor* test = Owner;
		ENetRole role = GetLocalRole();
		int bp = 0;
		ADummyMk3* Dummy = Cast<ADummyMk3>(hitActor);
		CurrentSelection = Dummy;
	}
	else if(CurrentSelection!=nullptr)
	{
		if (GetLocalRole() >= ROLE_AutonomousProxy)
		{
			CurrentSelection->MoveDummy(Hit.Location);
			CurrentSelection = nullptr;
		}
		//Server_MoveDummyTo(Hit.Location);
	}
	
}

void ATopDownCharacter::RightClick(const FInputActionValue& Value)
{
	FHitResult Hit;



	ATopDownPlayerController* PController = Cast<ATopDownPlayerController>(GetController());

	if (PController == nullptr)
		return;

	PController->GetHitResultUnderCursorByChannel(TraceTypeQuery1, false, Hit);

	Server_SpawnDummy(Hit.Location);
	if (TempDummy != nullptr)
		TempDummy->SetAutoRole();
	
}


// Called every frame
void ATopDownCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AActor* test = Owner;
	ENetRole role = GetLocalRole();
	int bp = 0;
}

void ATopDownCharacter::Server_SpawnDummy_Implementation(FVector Location)
{
	FVector SpawnLocation = Location + FVector(0, 0, 100);
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	ADummyMk3* dummy = GetWorld()->SpawnActor<ADummyMk3>(DummyClass, SpawnLocation, FRotator(0,0,0), SpawnParameters);
	dummy->Owner = this;
	TempDummy = dummy;
	//if (GetLocalRole() == ROLE_AutonomousProxy)
	//	dummy->SetAutoRole();
}

void ATopDownCharacter::Server_SpawnDummies_Implementation()
{
	TArray<AActor*> Locations;
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADummySpawnLocation::StaticClass(), Locations);
	for (int i = 0; i < Locations.Num(); i++)
	{
		ADummyMk3* dummy = GetWorld()->SpawnActor<ADummyMk3>(DummyClass, Locations[i]->GetActorLocation(), Locations[i]->GetActorRotation(), SpawnParameters);
		dummy->Owner = this;
	}
}

void ATopDownCharacter::Server_MoveDummyTo_Implementation(FVector Location)
{
	//CurrentSelection->Server_MoveTo(Location);
	//CurrentSelection = nullptr;
}

// Called to bind functionality to input
//void ATopDownCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
//{
//	Super::SetupPlayerInputComponent(PlayerInputComponent);
//
//	UEnhancedInputComponent* EnhancedInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
//	if(EnhancedInput!=nullptr)
//	{
//		EnhancedInput->BindAction(LeftClickAction, ETriggerEvent::Completed, this, &ATopDownCharacter::LeftClick);
//	}
//}

