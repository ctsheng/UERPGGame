// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystem/RPGAttributeSet.h"
#include "AbilitySystemInterface.h"
#include "RPGCharacter.generated.h"

class UAbilitySystemComponent;

UCLASS()
class UERPGGAME_API ARPGCharacter : public ACharacter, public IAbilitySystemInterface
{
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
	ARPGCharacter(const class FObjectInitializer& ObjectInitializer);

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    UFUNCTION(BlueprintCallable)
    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

    UFUNCTION(BlueprintCallable)
    virtual float GetHealth() const;

	UFUNCTION(BlueprintCallable)
	float GetMoveSpeed() const;

    UFUNCTION(BlueprintCallable)
    float GetMoveSpeedBaseValue() const;

    UFUNCTION(BlueprintCallable)
	virtual bool IsAlive() const;

    virtual void AddStartupEffects();

    virtual void HandleDamage(float DamageAmount, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags, ARPGCharacter* InstigatorPawn, AActor* DamageCauser);

	/**
	 * Attempts to activate all abilities that match the specified tags
	 * Returns true if it thinks it activated, but it may return false positives due to failure later in activation.
	 * If bAllowRemoteActivation is true, it will remotely activate local/server abilities, if false it will only try to locally activate the ability
	 */
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	bool ActivateAbilitiesWithTags(FGameplayTagContainer AbilityTags, bool bAllowRemoteActivation = true);

	/** Returns a list of active abilities matching the specified tags. This only returns if the ability is currently running */
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void GetActiveAbilitiesWithTags(FGameplayTagContainer AbilityTags, TArray<URPGGameplayAbility*>& ActiveAbilities);

	/** Returns total time and remaining time for cooldown tags. Returns false if no active cooldowns found */
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	bool GetCooldownRemainingForTag(FGameplayTagContainer CooldownTags, float& TimeRemaining, float& CooldownDuration);
protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "Melee")
    bool IsArmyed;

    /**
	 * Called when character takes damage, which may have killed them
	 *
	 * @param DamageAmount Amount of damage that was done, not clamped based on current health
	 * @param HitInfo The hit info that generated this damage
	 * @param DamageTags The gameplay tags of the event that did the damage
	 * @param InstigatorCharacter The character that initiated this damage
	 * @param DamageCauser The actual actor that did the damage, might be a weapon or projectile
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnDamaged(float DamageAmount, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags, ARPGCharacter* InstigatorCharacter, AActor* DamageCauser);



    TWeakObjectPtr<class URPGAbilitySystemComponent> AbilitySystemComponent;
    TWeakObjectPtr<class URPGAttributeSet> AttributeSet;


public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
    class AWeaponBase* CurrentWeapon;

    // These effects are only applied one time on startup
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<class UGameplayEffect>> StartupEffects;

};
