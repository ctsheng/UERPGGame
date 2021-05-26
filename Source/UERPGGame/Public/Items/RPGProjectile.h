// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffect.h"
#include "RPGProjectile.generated.h"

UCLASS()
class UERPGGAME_API ARPGProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARPGProjectile();

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float Range;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FGameplayEffectSpecHandle DamageEffectSpecHandle;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    class UProjectileMovementComponent* ProjectileMovement;

    UFUNCTION()
    void OnProjectileBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

    UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* ImpactVFX;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
