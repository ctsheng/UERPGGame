// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UERPGGame/UERPGGame.h"
#include "RPGCharacter.h"
#include "RPGAICharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterBaseHitReactDelegate, ERPGHitReactDirection, Direction);
/**
 *
 */
UCLASS()
class UERPGGAME_API ARPGAICharacter : public ARPGCharacter
{
    GENERATED_BODY()

public:
    ARPGAICharacter(const class FObjectInitializer& ObjectInitializer);

    virtual void BeginPlay() override;

    // Set the Hit React direction in the Animation Blueprint
    UPROPERTY(BlueprintAssignable, Category = "RPGAICharacter")
    FCharacterBaseHitReactDelegate ShowHitReact;

protected:

    virtual void HandleDamage(float DamageAmount, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags, ARPGCharacter* InstigatorPawn, AActor* DamageCauser) override;

    ERPGHitReactDirection GetHitReactDirection(const FVector& ImpactPoint);

    UFUNCTION(NetMulticast, Reliable, WithValidation)
    virtual void PlayHitReact(ERPGHitReactDirection HitDirection, AActor* DamageCauser);
    virtual void PlayHitReact_Implementation(ERPGHitReactDirection HitDirection, AActor* DamageCauser);
    virtual bool PlayHitReact_Validate(ERPGHitReactDirection HitDirection, AActor* DamageCauser);

    // Actual hard pointer to AbilitySystemComponent
    UPROPERTY()
    class URPGAbilitySystemComponent* HardRefAbilitySystemComponent;

    // Actual hard pointer to AttributeSetBase
    UPROPERTY()
    class URPGAttributeSet* HardRefAttributeSet;
};
