// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/RPGGameplayAbility.h"
#include "Items/RPGProjectile.h"
#include "RPGAbility_ProjectileBase.generated.h"

/**
 * 
 */
UCLASS()
class UERPGGAME_API URPGAbility_ProjectileBase : public URPGGameplayAbility
{
	GENERATED_BODY()
	
public:
   

    URPGAbility_ProjectileBase();

    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
    UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float Range;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float Damage; 
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = RPGGame)
    class UAnimMontage* MontageToPlay;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = RPGGame)
	TSubclassOf<ARPGProjectile> ProjectileClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = RPGGame)
    TSubclassOf<UGameplayEffect> DamageGameplayEffect;

    UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FName AttachedSocketName; 

	UFUNCTION()
	void OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	void OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	void EventReceived(FGameplayTag EventTag, FGameplayEventData EventData);
};
