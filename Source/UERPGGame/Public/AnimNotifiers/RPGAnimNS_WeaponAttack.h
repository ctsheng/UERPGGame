// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "GameplayTagContainer.h"
#include "RPGAnimNS_WeaponAttack.generated.h"

/**
 *
 */
UCLASS()
class UERPGGAME_API URPGAnimNS_WeaponAttack : public UAnimNotifyState
{
    GENERATED_BODY()

public:
    URPGAnimNS_WeaponAttack();

public:
    virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) override;
    virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;

public:
    float AttackDelayTime;
    int AttackDelayCount;
    FGameplayTag EventTag;
};
