// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/RPGAICharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystem/RPGAbilitySystemComponent.h"
#include "AbilitySystem/RPGAttributeSet.h"
#include "Components/CapsuleComponent.h"

ARPGAICharacter::ARPGAICharacter(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    PrimaryActorTick.bCanEverTick = true;

    // Create ability system component, and set it to be explicitly replicated
    HardRefAbilitySystemComponent = CreateDefaultSubobject<URPGAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
    HardRefAbilitySystemComponent->SetIsReplicated(true);

    // Minimal Mode means that no GameplayEffects will replicate. They will only live on the Server. Attributes, GameplayTags, and GameplayCues will still replicate to us.
    HardRefAbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

    // Set our parent's TWeakObjectPtr
    AbilitySystemComponent = HardRefAbilitySystemComponent;

    // Create the attribute set, this replicates by default
    // Adding it as a subobject of the owning actor of an AbilitySystemComponent
    // automatically registers the AttributeSet with the AbilitySystemComponent
    HardRefAttributeSet = CreateDefaultSubobject<URPGAttributeSet>(TEXT("AttributeSet"));

    // Set our parent's TWeakObjectPtr
    AttributeSet = HardRefAttributeSet;

    GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
}

void ARPGAICharacter::BeginPlay()
{
    Super::BeginPlay();

    AddStartupEffects();
    AttributeSet->SetHealth(10000);
    AttributeSet->SetMaxHealth(10000);
}

void ARPGAICharacter::HandleDamage(float DamageAmount, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags, ARPGCharacter* InstigatorPawn, AActor* DamageCauser)
{
    Super::HandleDamage(DamageAmount, HitInfo, DamageTags, InstigatorPawn, DamageCauser);

	FRootMotionSource_ConstantForce* ConstantForce = new FRootMotionSource_ConstantForce();
	ConstantForce->InstanceName = FName("ApplyRootMotionConstantForce");
	ConstantForce->AccumulateMode = ERootMotionAccumulateMode::Override;
	ConstantForce->Priority = 5;
	ConstantForce->Force = DamageCauser->GetActorForwardVector() * 200;
	ConstantForce->Duration = 0.1;
	ConstantForce->StrengthOverTime = nullptr;
	ConstantForce->FinishVelocityParams.Mode = ERootMotionFinishVelocityMode::MaintainLastRootMotionVelocity;
	ConstantForce->FinishVelocityParams.SetVelocity = FVector();
	ConstantForce->FinishVelocityParams.ClampVelocity = 0;
	int RootMotionSourceID = Cast<UCharacterMovementComponent>(GetMovementComponent())->ApplyRootMotionSource(ConstantForce);

    if (IsAlive())
    {
        if (HitInfo.IsValidBlockingHit())
        {
            ERPGHitReactDirection HitDirection = GetHitReactDirection(HitInfo.Location);
            PlayHitReact(HitDirection, InstigatorPawn);
        }
        else
        {
            // No hit result. Default to front.
            PlayHitReact(ERPGHitReactDirection::Front, InstigatorPawn);
        }
    }
}

ERPGHitReactDirection ARPGAICharacter::GetHitReactDirection(const FVector& ImpactPoint)
{
    const FVector& ActorLocation = GetActorLocation();
	// PointPlaneDist is super cheap - 1 vector subtraction, 1 dot product.
	float DistanceToFrontBackPlane = FVector::PointPlaneDist(ImpactPoint, ActorLocation, GetActorRightVector());
	float DistanceToRightLeftPlane = FVector::PointPlaneDist(ImpactPoint, ActorLocation, GetActorForwardVector());


	if (FMath::Abs(DistanceToFrontBackPlane) <= FMath::Abs(DistanceToRightLeftPlane))
	{
		// Determine if Front or Back

		// Can see if it's left or right of Left/Right plane which would determine Front or Back
		if (DistanceToRightLeftPlane >= 0)
		{
			return ERPGHitReactDirection::Front;
		}
		else
		{
			return ERPGHitReactDirection::Back;
		}
	}
	else
	{
		// Determine if Right or Left

		if (DistanceToFrontBackPlane >= 0)
		{
			return ERPGHitReactDirection::Right;
		}
		else
		{
			return ERPGHitReactDirection::Left;
		}
	}

	return ERPGHitReactDirection::Front;
}

void ARPGAICharacter::PlayHitReact_Implementation(ERPGHitReactDirection HitDirection, AActor* DamageCauser)
{
    if (IsAlive())
	{
        ShowHitReact.Broadcast(HitDirection);
	}
}

bool ARPGAICharacter::PlayHitReact_Validate(ERPGHitReactDirection HitDirection, AActor* DamageCauser)
{
    return true;
}

