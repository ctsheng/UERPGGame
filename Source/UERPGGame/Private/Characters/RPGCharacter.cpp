// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/RPGCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "AbilitySystem/RPGAbilitySystemComponent.h"
#include "Characters/RPGCharacterMovementComponent.h"

// Sets default values
ARPGCharacter::ARPGCharacter(const class FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer.SetDefaultSubobjectClass<URPGCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    bAlwaysRelevant = true;
}

// Called when the game starts or when spawned
void ARPGCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}


UAbilitySystemComponent* ARPGCharacter::GetAbilitySystemComponent() const
{
    return AbilitySystemComponent.Get();
}


float ARPGCharacter::GetHealth() const
{
    if (!AttributeSet.IsValid())
        return 1.f;

    return AttributeSet->GetHealth();
}

float ARPGCharacter::GetMoveSpeed() const
{
	if (AttributeSet.IsValid())
	{
		return AttributeSet->GetMoveSpeed();
	}

	return 0.0f;
}

float ARPGCharacter::GetMoveSpeedBaseValue() const
{
	if (AttributeSet.IsValid())
	{
		return AttributeSet->GetMoveSpeedAttribute().GetGameplayAttributeData(AttributeSet.Get())->GetBaseValue();
	}

	return 0.0f;
}

bool ARPGCharacter::IsAlive() const
{
    return GetHealth() > 0.0f;
}

void ARPGCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ARPGCharacter, IsArmyed);
}

bool ARPGCharacter::ActivateAbilitiesWithTags(FGameplayTagContainer AbilityTags, bool bAllowRemoteActivation)
{
    if (AbilitySystemComponent.IsValid())
    {
        return AbilitySystemComponent->TryActivateAbilitiesByTag(AbilityTags, bAllowRemoteActivation);
    }

    return false;
}

void ARPGCharacter::GetActiveAbilitiesWithTags(FGameplayTagContainer AbilityTags, TArray<URPGGameplayAbility*>& ActiveAbilities)
{
    if (AbilitySystemComponent.IsValid())
    {
        AbilitySystemComponent->GetActiveAbilitiesWithTags(AbilityTags, ActiveAbilities);
    }
}

bool ARPGCharacter::GetCooldownRemainingForTag(FGameplayTagContainer CooldownTags, float& TimeRemaining, float& CooldownDuration)
{
    if (AbilitySystemComponent.IsValid() && CooldownTags.Num() > 0)
    {
        TimeRemaining = 0.f;
        CooldownDuration = 0.f;

        FGameplayEffectQuery const Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTags);
        TArray< TPair<float, float> > DurationAndTimeRemaining = AbilitySystemComponent->GetActiveEffectsTimeRemainingAndDuration(Query);
        if (DurationAndTimeRemaining.Num() > 0)
        {
            int32 BestIdx = 0;
            float LongestTime = DurationAndTimeRemaining[0].Key;
            for (int32 Idx = 1; Idx < DurationAndTimeRemaining.Num(); ++Idx)
            {
                if (DurationAndTimeRemaining[Idx].Key > LongestTime)
                {
                    LongestTime = DurationAndTimeRemaining[Idx].Key;
                    BestIdx = Idx;
                }
            }

            TimeRemaining = DurationAndTimeRemaining[BestIdx].Key;
            CooldownDuration = DurationAndTimeRemaining[BestIdx].Value;

            return true;
        }
    }
    return false;
}


void ARPGCharacter::AddStartupEffects()
{
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid())
	{
		return;
	}

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	for (TSubclassOf<UGameplayEffect> GameplayEffect : StartupEffects)
	{
		FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, -1, EffectContext);
		if (NewHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent.Get());
		}
	}
}


void ARPGCharacter::HandleDamage(float DamageAmount, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags, ARPGCharacter* InstigatorPawn, AActor* DamageCauser)
{
	OnDamaged(DamageAmount, HitInfo, DamageTags, InstigatorPawn, DamageCauser);	
}

void ARPGCharacter::OnDamaged_Implementation(float DamageAmount, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags, ARPGCharacter* InstigatorCharacter, AActor* DamageCauser)
{

}