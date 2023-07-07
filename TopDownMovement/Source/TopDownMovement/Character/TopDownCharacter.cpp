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
	if(hitActor!= nullptr && hitActor->ActorHasTag("Dummy"))
	{
		ADummy* Dummy = Cast<ADummy>(hitActor);
		CurrentSelection = Dummy;
	}
	else if(CurrentSelection!=nullptr)
	{
		CurrentSelection->MoveTo(Hit.Location);
		CurrentSelection = nullptr;
	}
	

}


// Called every frame
void ATopDownCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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

