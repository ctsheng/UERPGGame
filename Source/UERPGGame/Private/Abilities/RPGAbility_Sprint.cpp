// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/RPGAbility_Sprint.h"
#include "Characters/RPGCharacter.h"
#include "Characters/RPGCharacterMovementComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitAttributeChangeThreshold.h"
#include "AbilitySystem/RPGAttributeSet.h"
#include "Player/RPGPlayerState.h"
#include "AbilityTasks/RPGAbilityTask_WaitVelocityBelowValue.h"

URPGAbility_Sprint::URPGAbility_Sprint()
{

}

void URPGAbility_Sprint::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* OwnerInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    if (HasAuthorityOrPredictionKey(OwnerInfo, &ActivationInfo))
    {
        if (!CommitAbility(Handle, OwnerInfo, ActivationInfo))
        {
            EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
        }

        GESprintEffects = ApplyEffectContainer(FGameplayTag::RequestGameplayTag(FName("Event.Ability.Sprint.Start")), FGameplayEventData());

        K2_AddGameplayCue(FGameplayTag::RequestGameplayTag(FName("GameplayCue.Character.Sprint")), FGameplayEffectContextHandle());

        ARPGCharacter* Char = CastChecked<ARPGCharacter>(OwnerInfo->AvatarActor.Get());
        if (Char)
        {
            CacheMovementComp = Cast<URPGCharacterMovementComponent>(Char->GetMovementComponent());
            if (CacheMovementComp)
            {
                CacheMovementComp->StartSprinting();
            }

            auto PS = Cast<ARPGPlayerState>(Char->GetPlayerState());
            if (PS)
            {
                URPGAbilityTask_WaitVelocityBelowValue* VelocityTask = URPGAbilityTask_WaitVelocityBelowValue::CreateWaitVelocityBelowValue(this, 10);
                VelocityTask->OnVelocityChage.AddDynamic(this, &URPGAbility_Sprint::ToStopSprinting);
                VelocityTask->ReadyForActivation();

                UAbilityTask_WaitAttributeChangeThreshold* ThredholdTask = UAbilityTask_WaitAttributeChangeThreshold::WaitForAttributeChangeThreshold(
                    this, PS->GetAttributeSetBase()->GetStaminaAttribute(), EWaitAttributeChangeComparison::LessThan, 5, true);
                ThredholdTask->OnChange.AddDynamic(this, &URPGAbility_Sprint::OnStaminaTooLow);
                ThredholdTask->ReadyForActivation(); 
            }       
        }
    }
}

void URPGAbility_Sprint::ToStopSprinting()
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void URPGAbility_Sprint::OnStaminaTooLow(bool IsTooLow, float Value)
{
    if (IsTooLow)
        EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void URPGAbility_Sprint::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    if (CacheMovementComp)
    {
        UE_LOG(LogTemp, Warning, TEXT("URPGAbility_Sprint::ToStopSprinting"));
        CacheMovementComp->StopSprinting();
    }

    BP_RemoveGameplayEffectFromOwnerWithAssetTags(FGameplayTagContainer(FGameplayTag::RequestGameplayTag(FName("Ability.Sprint.Cost"))));
    for (auto& GE : GESprintEffects)
    {
        BP_RemoveGameplayEffectFromOwnerWithHandle(GE);
    }

    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
