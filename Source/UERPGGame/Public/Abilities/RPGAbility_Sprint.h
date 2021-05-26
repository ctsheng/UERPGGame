// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/RPGGameplayAbility.h"
#include "RPGAbility_Sprint.generated.h"


/**
 * 
 */
UCLASS()
class UERPGGAME_API URPGAbility_Sprint : public URPGGameplayAbility
{
    GENERATED_BODY()

public:
    URPGAbility_Sprint();

    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* OwnerInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

    UPROPERTY()
    TArray<FActiveGameplayEffectHandle> GESprintEffects;

    UFUNCTION()
    void ToStopSprinting();

    UFUNCTION()
    void OnStaminaTooLow(bool IsTooLow, float Value);

    /** Native function, called if an ability ends normally or abnormally. If bReplicate is set to true, try to replicate the ending to the client/server */
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
    class URPGCharacterMovementComponent* CacheMovementComp;
};
