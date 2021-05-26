// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "RPGGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class UERPGGAME_API URPGGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
    virtual void Init() override;

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Inventory)
    FPrimaryAssetId DefaultWeapon;
};
