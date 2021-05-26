// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/RPGGameplayAbility.h"
#include "RPGAbility_Dash.generated.h"

class URPGAbilityTask_PlayMontageAndWaitForEvent;

/**
 *
 */
UCLASS()
class UERPGGAME_API URPGAbility_Dash : public URPGGameplayAbility
{
    GENERATED_BODY()

protected:

    UFUNCTION()
    void OnMotionFinish();

    URPGAbilityTask_PlayMontageAndWaitForEvent* MontageTask;

public:

    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* OwnerInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

    UPROPERTY(EditDefaultsOnly, Category = MontageAbility)
        UAnimMontage *	MontageToPlay;

    UFUNCTION()
        void OnCanceled(FGameplayTag Tag, FGameplayEventData Data);

    UPROPERTY(BlueprintReadOnly, EditAnywhere)
        TSubclassOf<UGameplayEffect> DamageGameplayEffect;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Ability)
        float Strength;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Ability)
        float DashTime;

};
