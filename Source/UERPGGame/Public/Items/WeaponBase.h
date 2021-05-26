// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "WeaponBase.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FRPGWeaponAttackAnimNotifyDelegate, FGameplayTag, EventTag, int, AttackDelayTime, float, AttackDelayCount);

UCLASS()
class UERPGGAME_API AWeaponBase : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AWeaponBase();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UFUNCTION()
        void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

protected:

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
        class UStaticMeshComponent* MeshComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
        class UCapsuleComponent* CollisionComp;


public:
    UPROPERTY()
        bool IsAttacking;

    //UPROPERTY()
    //    FRPGWeaponAttackAnimNotifyDelegate OnWeaponAttackBegin;

    //UPROPERTY()
    //    FRPGWeaponAttackAnimNotifyDelegate OnWeaponAttackEnd;


    void OnWeaponAttackBegin(FGameplayTag EventTag, int AttackDelayTime, float AttackDelayCount);

    void OnWeaponAttackEnd();

public:
    FGameplayTag EventTag;
    int AttackDelayTime;
    float AttackDelayCount;

};
