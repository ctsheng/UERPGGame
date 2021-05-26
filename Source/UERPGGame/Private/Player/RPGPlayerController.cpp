// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/RPGPlayerController.h"
#include "RPGGameInstance.h"
#include "Characters/RPGCharacter.h"
#include "Items/WeaponBase.h"
#include "Items/RPGWeaponItem.h"
#include "Player/RPGPlayerState.h"


void ARPGPlayerController::BeginPlay()
{
    Super::BeginPlay();
}

void ARPGPlayerController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    ARPGPlayerState* PS = GetPlayerState<ARPGPlayerState>();
    if (PS)
    {
        // Init ASC with PS (Owner) and our new Pawn (AvatarActor)
        PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, InPawn);
    }
}

void ARPGPlayerController::BeginPlayingState()
{
    BlueprintBeginPlayingState();
}

