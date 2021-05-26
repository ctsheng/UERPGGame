// Copyright Epic Games, Inc. All Rights Reserved.

#include "AbilitySystem/RPGTargetType.h"
#include "AbilitySystem/RPGGameplayAbility.h"
#include "Characters/RPGCharacter.h"

void URPGTargetType::GetTargets_Implementation(ARPGCharacter* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const
{
	return;
}

void URPGTargetType_UseOwner::GetTargets_Implementation(ARPGCharacter* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const
{
	OutActors.Add(TargetingCharacter);
}

void URPGTargetType_UseEventData::GetTargets_Implementation(ARPGCharacter* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const
{
	const FHitResult* FoundHitResult = EventData.ContextHandle.GetHitResult();
	if (FoundHitResult)
	{
		OutHitResults.Add(*FoundHitResult);
	}
	else if (EventData.Target)
	{
		OutActors.Add(const_cast<AActor*>(EventData.Target));
	}
}