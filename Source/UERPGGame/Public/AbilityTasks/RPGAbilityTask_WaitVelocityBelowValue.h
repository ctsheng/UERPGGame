// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask_WaitVelocityChange.h"
#include "RPGAbilityTask_WaitVelocityBelowValue.generated.h"

/**
 * 
 */
UCLASS()
class UERPGGAME_API URPGAbilityTask_WaitVelocityBelowValue : public UAbilityTask_WaitVelocityChange
{
	GENERATED_UCLASS_BODY()

    static URPGAbilityTask_WaitVelocityBelowValue* CreateWaitVelocityBelowValue(UGameplayAbility* OwningAbility, float InMinimumMagnitude);

protected:
    virtual void TickTask(float DeltaTime) override;	
};
