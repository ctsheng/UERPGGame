// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/RPGPlayerState.h"
#include "AbilitySystem/RPGAbilitySystemComponent.h"
#include "AbilitySystem/RPGAttributeSet.h"
#include "Characters/RPGAICharacter.h"

ARPGPlayerState::ARPGPlayerState()
{
    // Create ability system component, and set it to be explicitly replicated
    AbilitySystemComponent = CreateDefaultSubobject<URPGAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
    AbilitySystemComponent->SetIsReplicated(true);

    // Mixed mode means we only are replicated the GEs to ourself, not the GEs to simulated proxies. If another GDPlayerState (Hero) receives a GE,
    // we won't be told about it by the Server. Attributes, GameplayTags, and GameplayCues will still replicate to us.
    AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

    // Create the attribute set, this replicates by default
    // Adding it as a subobject of the owning actor of an AbilitySystemComponent
    // automatically registers the AttributeSet with the AbilitySystemComponent
    AttributeSet = CreateDefaultSubobject<URPGAttributeSet>(TEXT("AttributeSet"));

    // Set PlayerState's NetUpdateFrequency to the same as the Character.
    // Default is very low for PlayerStates and introduces perceived lag in the ability system.
    // 100 is probably way too high for a shipping game, you can adjust to fit your needs.
    NetUpdateFrequency = 100.0f;
}

void ARPGPlayerState::BeginPlay()
{
    Super::BeginPlay();

    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddUObject(this, &ARPGPlayerState::HealthChanged);
    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetManaAttribute()).AddUObject(this, &ARPGPlayerState::ManaChanged);
    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetStaminaAttribute()).AddUObject(this, &ARPGPlayerState::StaminaChanged);

    // test code
    AttributeSet->SetHealth(100);
    AttributeSet->SetMaxHealth(100);
    AttributeSet->SetMaxMana(100);
    AttributeSet->SetMana(100);
    AttributeSet->SetMaxStamina(100);
    AttributeSet->SetStamina(100);
    AttributeSet->SetMoveSpeed(600);

    AbilitySystemComponent->OnAnyGameplayEffectRemovedDelegate().AddUObject(this, &ARPGPlayerState::OnGameplayRemove);
    AbilitySystemComponent->OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &ARPGPlayerState::OnGameplayApply);
}

void ARPGPlayerState::OnGameplayApply(UAbilitySystemComponent* asc, const FGameplayEffectSpec& effect, FActiveGameplayEffectHandle handle)
{
    UE_LOG(LogTemp, Warning, TEXT("OnGameplayApply %s"), *(effect.ToSimpleString()));
}

void ARPGPlayerState::OnGameplayRemove(const FActiveGameplayEffect& Data)
{
    UE_LOG(LogTemp, Warning, TEXT("OnGameplayRemove %s"), *(Data.Spec.ToSimpleString()));

}

class UAbilitySystemComponent* ARPGPlayerState::GetAbilitySystemComponent() const
{
    return AbilitySystemComponent;
}

class URPGAttributeSet* ARPGPlayerState::GetAttributeSetBase() const
{
    return AttributeSet;
}

float ARPGPlayerState::GetHealth() const
{
    return AttributeSet->GetHealth();
}

float ARPGPlayerState::GetMaxHealth() const
{
    return AttributeSet->GetMaxHealth();
}

float ARPGPlayerState::GetMana() const
{
    return AttributeSet->GetMana();
}

float ARPGPlayerState::GetMaxMana() const
{
    return AttributeSet->GetMaxMana();
}

float ARPGPlayerState::GetStamina() const
{
    return AttributeSet->GetStamina();
}

float ARPGPlayerState::GetMaxStamina() const
{
    return AttributeSet->GetMaxStamina();
}

bool ARPGPlayerState::IsAlive() const
{
    return GetHealth() > 0.0;
}

void ARPGPlayerState::HealthChanged(const FOnAttributeChangeData& Data)
{
    OnHealthChangeDelegate.Broadcast(GetHealth());
}

void ARPGPlayerState::MaxHealthChanged(const FOnAttributeChangeData& Data)
{

}

void ARPGPlayerState::ManaChanged(const FOnAttributeChangeData& Data)
{
    OnManaChangeDelegate.Broadcast(GetMana());
}

void ARPGPlayerState::MaxManaChanged(const FOnAttributeChangeData& Data)
{

}

void ARPGPlayerState::StaminaChanged(const FOnAttributeChangeData& Data)
{
    OnStaminaChangeDelegate.Broadcast(GetStamina());
}

void ARPGPlayerState::MaxStaminaChanged(const FOnAttributeChangeData& Data)
{

}
