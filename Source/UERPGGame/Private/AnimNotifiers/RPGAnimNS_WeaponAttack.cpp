// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifiers/RPGAnimNS_WeaponAttack.h"
#include "Items/WeaponBase.h"
#include "Characters/RPGCharacter.h"

URPGAnimNS_WeaponAttack::URPGAnimNS_WeaponAttack()
{
    AttackDelayTime = 0.4;
    AttackDelayCount = 2;
    EventTag = FGameplayTag::RequestGameplayTag(FName("Event.Montage.Shared.WeaponHit"));
}

void URPGAnimNS_WeaponAttack::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
    Super::NotifyBegin(MeshComp, Animation, TotalDuration);

    AActor* Owner = MeshComp->GetOwner();
    ARPGCharacter* Character = Cast<ARPGCharacter>(Owner);

    if (Character && Character->CurrentWeapon)
    {
        Character->CurrentWeapon->OnWeaponAttackBegin(EventTag, AttackDelayTime, AttackDelayTime);
    }
}

void URPGAnimNS_WeaponAttack::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
    Super::NotifyEnd(MeshComp, Animation);

    AActor* Owner = MeshComp->GetOwner();
    ARPGCharacter* Character = Cast<ARPGCharacter>(Owner);

    if (Character && Character->CurrentWeapon)
    {
        Character->CurrentWeapon->OnWeaponAttackEnd();
    }
}
