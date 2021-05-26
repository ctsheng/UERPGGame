// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/RPGGameplayAbility.h"
#include "RPGAbility_MeleeBase.generated.h"

/**
 * 
 */
UCLASS()
class UERPGGAME_API URPGAbility_MeleeBase : public URPGGameplayAbility
{
	GENERATED_BODY()

public:

    URPGAbility_MeleeBase();

    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* OwnerInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

    UPROPERTY(EditDefaultsOnly, Category = MontageAbility)
        UAnimMontage *	MontageToPlay;
	
    UFUNCTION()
        void OnCompleted(FGameplayTag Tag, FGameplayEventData Data);

    UFUNCTION()
        void OnCanceled(FGameplayTag Tag, FGameplayEventData Data);

    UFUNCTION()
        void EventReceived(FGameplayTag EventTag, FGameplayEventData Data);

    UPROPERTY(BlueprintReadOnly, EditAnywhere)
        TSubclassOf<UGameplayEffect> DamageGameplayEffect;
};
