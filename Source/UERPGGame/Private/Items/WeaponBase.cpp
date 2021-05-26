// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/WeaponBase.h"
#include "Components/CapsuleComponent.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "AbilitySystemBlueprintLibrary.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    IsAttacking = false;

    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
    MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    RootComponent = MeshComp;

    CollisionComp = CreateDefaultSubobject<UCapsuleComponent>("Collision");
    CollisionComp->SetupAttachment(MeshComp);

    CollisionComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    CollisionComp->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);
    CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
    CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AWeaponBase::OnActorOverlap);
}

void AWeaponBase::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    AActor* WeaponInstigator = GetInstigator();
    if (WeaponInstigator != OtherActor && IsAttacking)
    {
        FGameplayEventData Payload;
        Payload.EventTag = EventTag;
        Payload.Instigator = GetInstigator();
        Payload.Target = OtherActor;
        UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(WeaponInstigator, EventTag, Payload);
    }
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponBase::OnWeaponAttackBegin(FGameplayTag Tag, int Delaytime, float DelayCount)
{
    IsAttacking = true;
    EventTag = Tag;
    AttackDelayTime = Delaytime;
    AttackDelayCount = DelayCount;

    CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AWeaponBase::OnWeaponAttackEnd()
{
    IsAttacking = false;
    CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

