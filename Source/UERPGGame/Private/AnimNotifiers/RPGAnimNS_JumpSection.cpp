// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifiers/RPGAnimNS_JumpSection.h"
#include "Characters/PlayerCharacter.h"

void URPGAnimNS_JumpSection::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
    APlayerCharacter* Char = Cast<APlayerCharacter>(MeshComp->GetOwner());

    if (Char)
    {
        Char->SetComboPeriodEnable(true);
    }
}

void URPGAnimNS_JumpSection::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
    APlayerCharacter* Char = Cast<APlayerCharacter>(MeshComp->GetOwner());

    if (Char)
    {
        Char->SetComboPeriodEnable(false);
    }
}
