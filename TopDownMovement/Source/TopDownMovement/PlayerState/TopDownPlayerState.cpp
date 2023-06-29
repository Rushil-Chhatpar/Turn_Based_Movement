// Fill out your copyright notice in the Description page of Project Settings.


#include "TopDownMovement/PlayerState/TopDownPlayerState.h"
#include "Net/UnrealNetwork.h"



void ATopDownPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const 
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ATopDownPlayerState, PlayerTeamID);
}