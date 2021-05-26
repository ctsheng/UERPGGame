// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/RPGAbility_ProjectileBase.h"
#include "AbilityTasks/RPGAbilityTask_PlayMontageAndWaitForEvent.h"
#include "Characters/PlayerCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

URPGAbility_ProjectileBase::URPGAbility_ProjectileBase()
{
    Range = 1000.0f;
	Damage = 12.0f;
}

void URPGAbility_ProjectileBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
    {
        if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
        {
            EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
        }

        APlayerCharacter* Char = Cast<APlayerCharacter>(ActorInfo->AvatarActor.Get());
        if (Char)
        {
            auto Enemy = Char->TryGetNearestEnemy();
            {
                if (Enemy)
                {
                    FRotator Rotator = UKismetMathLibrary::FindLookAtRotation(Char->GetActorLocation(), Enemy->GetActorLocation());
                    Char->SetActorRotation(Rotator);
                }
            }
        }

        FGameplayTagContainer MontageTag;
        MontageTag.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Event.Montage")));

        URPGAbilityTask_PlayMontageAndWaitForEvent* Task =
            URPGAbilityTask_PlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(
                this, NAME_None, MontageToPlay, MontageTag);
        Task->OnBlendOut.AddDynamic(this, &URPGAbility_ProjectileBase::OnCompleted);
        Task->OnCompleted.AddDynamic(this, &URPGAbility_ProjectileBase::OnCompleted);
        Task->OnInterrupted.AddDynamic(this, &URPGAbility_ProjectileBase::OnCancelled);
        Task->OnCancelled.AddDynamic(this, &URPGAbility_ProjectileBase::OnCancelled);
        Task->EventReceived.AddDynamic(this, &URPGAbility_ProjectileBase::EventReceived);
        Task->ReadyForActivation();
    }
}

void URPGAbility_ProjectileBase::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void URPGAbility_ProjectileBase::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void URPGAbility_ProjectileBase::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
    // Montage told us to end the ability before the montage finished playing.
	// Montage was set to continue playing animation even after ability ends so this is okay.
	if (EventTag == FGameplayTag::RequestGameplayTag(FName("Event.Montage.EndAbility")))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

    // Only spawn projectiles on the Server.
	// Predicting projectiles is an advanced topic not covered in this example.
    if (GetOwningActorFromActorInfo()->GetLocalRole() == ROLE_Authority && EventTag == FGameplayTag::RequestGameplayTag(FName("Event.Montage.SpawnProjectile")))
    {
        APlayerCharacter* Char = Cast<APlayerCharacter>(GetAvatarActorFromActorInfo());
		if (!Char)
		{
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		}

        FGameplayEffectSpecHandle DamageEffectSpecHandle = MakeOutgoingGameplayEffectSpec(DamageGameplayEffect, GetAbilityLevel());
		DamageEffectSpecHandle.Data.Get()->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage")), Damage);

		FTransform MuzzleTransform = Char->GetMesh()->GetSocketTransform(AttachedSocketName);
		MuzzleTransform.SetRotation(Char->GetActorRotation().Quaternion());
		MuzzleTransform.SetScale3D(FVector(1.0f));

		ARPGProjectile* Projectile = GetWorld()->SpawnActorDeferred<ARPGProjectile>(ProjectileClass, MuzzleTransform, GetOwningActorFromActorInfo(),
			Char, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		Projectile->DamageEffectSpecHandle = DamageEffectSpecHandle;
		Projectile->Range = Range;
		Projectile->FinishSpawning(MuzzleTransform);
    }
}
