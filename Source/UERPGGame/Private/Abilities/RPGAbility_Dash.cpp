// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/RPGAbility_Dash.h"
#include "Abilities/Tasks/AbilityTask_ApplyRootMotionConstantForce.h"
#include "Characters/RPGCharacter.h"
#include "AbilitySystem/RPGAbilitySystemComponent.h"
#include "AbilityTasks/RPGAbilityTask_PlayMontageAndWaitForEvent.h"
#include "Characters/PlayerCharacter.h"
#include "Player/RPGPlayerState.h"


void URPGAbility_Dash::OnMotionFinish()
{
    if (MontageTask)
    {
        MontageTask->EndTask();
    }

    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);

    APlayerCharacter* Char = Cast<APlayerCharacter>(GetAvatarActorFromActorInfo());
    if (Char)
    {
        Char->Sprint();
    }  
}

void URPGAbility_Dash::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* OwnerInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    if (HasAuthorityOrPredictionKey(OwnerInfo, &ActivationInfo))
    {
        if (!CommitAbility(Handle, OwnerInfo, ActivationInfo))
        {
            EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
        }

        ApplyEffectContainer(FGameplayTag::RequestGameplayTag("Event.Ability.Dash.Start"), FGameplayEventData());
        ARPGCharacter* Char = Cast<ARPGCharacter>(OwnerInfo->AvatarActor.Get());
        if (Char)
        {
            FRotator ControllerRotation = Char->GetControlRotation();
            FRotator CurrentRotation = Char->GetActorRotation();
            CurrentRotation.Yaw = ControllerRotation.Yaw;
            Char->SetActorRotation(CurrentRotation);
        }

        FGameplayTagContainer MontageTag;
        MontageTag.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Event.Montage")));
        MontageTask = URPGAbilityTask_PlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(this, NAME_None, MontageToPlay, MontageTag);
        MontageTask->ReadyForActivation();

        auto MotionTask = UAbilityTask_ApplyRootMotionConstantForce::ApplyRootMotionConstantForce(this, "Dash", Char->GetActorForwardVector(),
            Strength, DashTime, false, nullptr, ERootMotionFinishVelocityMode::MaintainLastRootMotionVelocity, FVector(), 0);

        MotionTask->ReadyForActivation();
        MotionTask->OnFinish.AddDynamic(this, &URPGAbility_Dash::OnMotionFinish);
    }
}

void URPGAbility_Dash::OnCanceled(FGameplayTag Tag, FGameplayEventData Data)
{

}
