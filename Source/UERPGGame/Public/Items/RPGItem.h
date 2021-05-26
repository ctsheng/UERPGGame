// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RPGAssetManager.h"
#include "RPGItem.generated.h"

/**
 *
 */
UCLASS()
class UERPGGAME_API URPGItem : public UPrimaryDataAsset
{
    GENERATED_BODY()


public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
        FPrimaryAssetType ItemType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
        FText ItemName;


public:
    virtual FPrimaryAssetId GetPrimaryAssetId() const override;

};
