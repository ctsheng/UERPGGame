// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "RPGPlayerState.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRPGCharacterBaseAttrChangeDelegate, float, NewValue);
/**
 *
 */
UCLASS()
class UERPGGAME_API ARPGPlayerState : public APlayerState, public IAbilitySystemInterface
{
    GENERATED_BODY()

public:
    ARPGPlayerState();

    virtual void BeginPlay() override;

    // Implement IAbilitySystemInterface
    class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

    class URPGAttributeSet* GetAttributeSetBase() const;

    UPROPERTY(BlueprintAssignable, Category = "Attributes")
    FRPGCharacterBaseAttrChangeDelegate OnHealthChangeDelegate;

    UPROPERTY(BlueprintAssignable, Category = "Attributes")
    FRPGCharacterBaseAttrChangeDelegate OnManaChangeDelegate;

    UPROPERTY(BlueprintAssignable, Category = "Attributes")
    FRPGCharacterBaseAttrChangeDelegate OnStaminaChangeDelegate;


    /**
	* Getters for attributes from GDAttributeSetBase. Returns Current Value unless otherwise specified.
	*/
	UFUNCTION(BlueprintCallable, Category = "GASDocumentation|GDPlayerState|Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "GASDocumentation|GDPlayerState|Attributes")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "GASDocumentation|GDPlayerState|Attributes")
	float GetMana() const;

	UFUNCTION(BlueprintCallable, Category = "GASDocumentation|GDPlayerState|Attributes")
	float GetMaxMana() const;

	UFUNCTION(BlueprintCallable, Category = "GASDocumentation|GDPlayerState|Attributes")
	float GetStamina() const;

	UFUNCTION(BlueprintCallable, Category = "GASDocumentation|GDPlayerState|Attributes")
	float GetMaxStamina() const;

    UFUNCTION(BlueprintCallable, Category = "RPGGame|RPGPlayerState")
	virtual bool IsAlive() const;

protected:
    // Attribute changed callbacks
	virtual void HealthChanged(const FOnAttributeChangeData& Data);
	virtual void MaxHealthChanged(const FOnAttributeChangeData& Data);
	virtual void ManaChanged(const FOnAttributeChangeData& Data);
	virtual void MaxManaChanged(const FOnAttributeChangeData& Data);
	virtual void StaminaChanged(const FOnAttributeChangeData& Data);
	virtual void MaxStaminaChanged(const FOnAttributeChangeData& Data);

    void OnGameplayRemove(const FActiveGameplayEffect& Data);
    void OnGameplayApply(UAbilitySystemComponent* asc, const FGameplayEffectSpec& effect, FActiveGameplayEffectHandle handle);

protected:
    UPROPERTY()
        class URPGAbilitySystemComponent* AbilitySystemComponent;

    UPROPERTY()
        class URPGAttributeSet* AttributeSet;
};
