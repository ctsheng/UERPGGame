// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"

URPGAnimInstance::URPGAnimInstance()
    : IsAccelerating(false)
    , FullBody(false)
    , IsInAir(false)
    , Speed(0.0f)
{

}

void URPGAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    ACharacter* Char = Cast<ACharacter>(TryGetPawnOwner());

    if (Char)
    {
        UCharacterMovementComponent* Movement = Cast<UCharacterMovementComponent>(Char->GetMovementComponent());

        if (Movement)
        {
            IsInAir = Movement->IsFalling();
            IsAccelerating = Movement->GetCurrentAcceleration().Size() > 0;
        }

        FullBody = GetCurveValue("FullBody") > 0;
        Speed = Char->GetVelocity().Size();
    }
}
