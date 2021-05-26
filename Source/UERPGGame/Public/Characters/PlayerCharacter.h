// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPGCharacter.h"
#include "Abilities/RPGAbility_MeleeBase.h"
#include "PlayerCharacter.generated.h"

class URPGGameplayAbility;

/**
 *
 */
UCLASS()
class UERPGGAME_API APlayerCharacter : public ARPGCharacter
{
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
    APlayerCharacter(const class FObjectInitializer& ObjectInitializer);

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    class USpringArmComponent* GetCameraBoom();

    class UCameraComponent* GetFollowCamera();

    /**Combat */
    void SetComboPeriodEnable(bool IsEnable) { IsEnabledComboPeriod = IsEnable; }
  
    void Sprint();

    AActor* TryGetNearestEnemy();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Character Movement
    void MoveForward(float Value);
    void MoveRight(float Value);

    // Character Action Binded
    void JumpPressedAction();    
    void DoPrimaryAttack();
    void Dash();
    void DoSkill();

    // GAS Initial
    // Client only
    virtual void OnRep_PlayerState() override;
    // Only called on the Server. Calls before Server's AcknowledgePossession.
    virtual void PossessedBy(AController* NewController) override;
    void InitPlayerAbilitySystem();

    // Abilities Initial
    void AddStartupGameplayAbilities();

    // ANIM
    bool GetIsFullBodyAnimating();

    bool IsUsingMelee();

    /** Combat Combo */
    TSubclassOf<URPGAbility_MeleeBase> GetPrimaryAttackCombo(int Count);

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    class USpringArmComponent* CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    class UCameraComponent* FollowCamera;

    UPROPERTY()
    bool IsEnabledComboPeriod;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Ability)
    TArray<TSubclassOf<URPGAbility_MeleeBase>> GAPrimaryAttacks;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Ability)
    TSubclassOf<URPGGameplayAbility> GADash;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Ability)
    TSubclassOf<URPGGameplayAbility> GASprint;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Ability)
    TSubclassOf<URPGGameplayAbility> GASkill;

    UPROPERTY()
    int AttackCount;

    UPROPERTY()
    FGameplayAbilitySpecHandle GAPrimaryAttackSpecHandle;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement | Mantle")
    class UALSMantleComponent* MantleComponent;
    
};
