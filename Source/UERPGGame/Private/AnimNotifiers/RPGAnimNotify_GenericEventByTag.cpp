// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifiers/RPGAnimNotify_GenericEventByTag.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Actor.h"

void URPGAnimNotify_GenericEventByTag::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    AActor* Actor = MeshComp->GetOwner();
    UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Actor, EventTag, FGameplayEventData());
}
