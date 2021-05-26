// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/RPGItem.h"
#include "RPGWeaponItem.generated.h"

/** Native base class for weapons, should be blueprinted */
UCLASS()
class UERPGGAME_API URPGWeaponItem : public URPGItem
{
    GENERATED_BODY()

public:
    /** Constructor */
    URPGWeaponItem()
    {
        ItemType = URPGAssetManager::WeaponItemType;
    }

    /** Weapon actor to spawn */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
        TSubclassOf<AActor> WeaponActor;
};