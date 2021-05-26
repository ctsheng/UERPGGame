// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/RPGAbility_MeleeBase.h"
#include "Characters/PlayerCharacter.h"
#include "AbilityTasks/RPGAbilityTask_PlayMontageAndWaitForEvent.h"
#include "GameplayTagContainer.h"

URPGAbility_MeleeBase::URPGAbility_MeleeBase()
{
    AbilityTags.AddTag(FGameplayTag::RequestGameplayTag("Ability.Melee"));
}

void URPGAbility_MeleeBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* OwnerInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    if (HasAuthorityOrPredictionKey(OwnerInfo, &ActivationInfo))
    {
        if (!CommitAbility(Handle, OwnerInfo, ActivationInfo))
        {
            EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
        }
     
        APlayerCharacter* Char = Cast<APlayerCharacter>(OwnerInfo->AvatarActor.Get());
        if (Char)
        {
            FRotator ControllerRotation = Char->GetControlRotation();
            FRotator CurrentRotation = Char->GetActorRotation();
            CurrentRotation.Yaw = ControllerRotation.Yaw;
            Char->SetActorRotation(CurrentRotation);
        }

        FGameplayTagContainer MontageTag;
        MontageTag.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Event.Montage")));

        URPGAbilityTask_PlayMontageAndWaitForEvent* Task =
            URPGAbilityTask_PlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(
                this, NAME_None, MontageToPlay, MontageTag);
        Task->OnBlendOut.AddDynamic(this, &URPGAbility_MeleeBase::OnCompleted);
        Task->OnCompleted.AddDynamic(this, &URPGAbility_MeleeBase::OnCompleted);
        Task->OnInterrupted.AddDynamic(this, &URPGAbility_MeleeBase::OnCanceled);
        Task->OnCancelled.AddDynamic(this, &URPGAbility_MeleeBase::OnCanceled);
        Task->EventReceived.AddDynamic(this, &URPGAbility_MeleeBase::EventReceived);
        Task->ReadyForActivation();
    }
}

void URPGAbility_MeleeBase::OnCompleted(FGameplayTag Tag, FGameplayEventData Data)
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void URPGAbility_MeleeBase::OnCanceled(FGameplayTag Tag, FGameplayEventData Data)
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void URPGAbility_MeleeBase::EventReceived(FGameplayTag EventTag, FGameplayEventData Data)
{
    if (EventTag == FGameplayTag::RequestGameplayTag(FName("Event.Montage.EndAbility")))
    {
        EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
        return;
    }

    ApplyEffectContainer(EventTag, Data, -1);   
}

