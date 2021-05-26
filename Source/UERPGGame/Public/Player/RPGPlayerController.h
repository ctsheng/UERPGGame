// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RPGPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UERPGGAME_API ARPGPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
    ARPGPlayerController() {};

protected:
    virtual void BeginPlay() override;

    virtual void OnPossess(APawn* InPawn) override;

    /* Called when player controller is ready to begin playing, good moment to initialize things like UI which might be too early in BeginPlay 
		(esp. in multiplayer clients where not all data such as PlayerState may have been received yet) */
	virtual void BeginPlayingState() override;

	UFUNCTION(BlueprintImplementableEvent)
	void BlueprintBeginPlayingState();


public:
    UPROPERTY()
        class URPGWeaponItem* InitialWeapon;
};
