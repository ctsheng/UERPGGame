// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/RPGItem.h"

FPrimaryAssetId URPGItem::GetPrimaryAssetId() const
{
    return FPrimaryAssetId(ItemType, GetFName());
}
