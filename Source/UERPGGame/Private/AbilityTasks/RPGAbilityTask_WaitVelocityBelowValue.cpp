// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityTasks/RPGAbilityTask_WaitVelocityBelowValue.h"
#include "GameFramework/MovementComponent.h"

URPGAbilityTask_WaitVelocityBelowValue::URPGAbilityTask_WaitVelocityBelowValue(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void URPGAbilityTask_WaitVelocityBelowValue::TickTask(float DeltaTime)
{
    UE_LOG(LogTemp, Warning, TEXT("CachedMovementComponent %d"), CachedMovementComponent);
    if (CachedMovementComponent)
	{
        float Velocity = CachedMovementComponent->Velocity.Size();
        UE_LOG(LogTemp, Warning, TEXT("Velocity %s %f"), *(CachedMovementComponent->Velocity.ToString()), MinimumMagnitude);

		if (Velocity < MinimumMagnitude)
		{
			if (ShouldBroadcastAbilityTaskDelegates())
			{
				OnVelocityChage.Broadcast();
			}
			EndTask();
		}
	}
	else
	{
		ABILITY_LOG(Warning, TEXT("UAbilityTask_WaitVelocityChange ticked without a valid movement component. ending."));
		EndTask();
	}
}


URPGAbilityTask_WaitVelocityBelowValue* URPGAbilityTask_WaitVelocityBelowValue::CreateWaitVelocityBelowValue(UGameplayAbility* OwningAbility, float InMinimumMagnitude)
{
	URPGAbilityTask_WaitVelocityBelowValue* MyObj = NewAbilityTask<URPGAbilityTask_WaitVelocityBelowValue>(OwningAbility);
	MyObj->MinimumMagnitude = InMinimumMagnitude;
	return MyObj;
}
