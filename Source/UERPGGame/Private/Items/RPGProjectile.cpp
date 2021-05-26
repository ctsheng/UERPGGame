// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/RPGProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Characters/RPGCharacter.h"
#include "AbilitySystem/RPGAbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ARPGProjectile::ARPGProjectile()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    bReplicates = true;

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(FName("ProjectileMovement"));
}

// Called when the game starts or when spawned
void ARPGProjectile::BeginPlay()
{
    Super::BeginPlay();

    SetLifeSpan(Range / ProjectileMovement->InitialSpeed);

    OnActorBeginOverlap.AddDynamic(this, &ARPGProjectile::OnProjectileBeginOverlap);
}

// Called every frame
void ARPGProjectile::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}


void ARPGProjectile::OnProjectileBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
    if (GetInstigator() == OtherActor)
    {
        return;
    }

    ARPGCharacter* HitActor = Cast<ARPGCharacter>(OtherActor);

    if (GetLocalRole() != ROLE_Authority || !HitActor)
    {
        Destroy();
        return;
    }

    URPGAbilitySystemComponent* ASC = Cast<URPGAbilitySystemComponent>(HitActor->GetAbilitySystemComponent());
    if (ASC)
    {
        ASC->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());
    }

    UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());

    Destroy();
}



