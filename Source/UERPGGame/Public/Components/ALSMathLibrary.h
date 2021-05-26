// Project:         Advanced Locomotion System V4 on C++
// Copyright:       Copyright (C) 2021 Doğa Can Yanıkoğlu
// License:         MIT License (http://www.opensource.org/licenses/mit-license.php)
// Source Code:     https://github.com/dyanikoglu/ALSV4_CPP
// Original Author: Doğa Can Yanıkoğlu
// Contributors:    Achim Turan


#pragma once

#include "Kismet/KismetSystemLibrary.h"
#include "CoreMinimal.h"
#include "Curves/CurveVector.h"
#include "ALSMathLibrary.generated.h"

class UCapsuleComponent;

USTRUCT(BlueprintType)
struct FALSMantleTraceSettings
{
    GENERATED_BODY()

        UPROPERTY(EditAnywhere, Category = "Mantle System")
        float MaxLedgeHeight = 0.0f;

    UPROPERTY(EditAnywhere, Category = "Mantle System")
        float MinLedgeHeight = 0.0f;

    UPROPERTY(EditAnywhere, Category = "Mantle System")
        float ReachDistance = 0.0f;

    UPROPERTY(EditAnywhere, Category = "Mantle System")
        float ForwardTraceRadius = 0.0f;

    UPROPERTY(EditAnywhere, Category = "Mantle System")
        float DownwardTraceRadius = 0.0f;
};


UENUM(BlueprintType)
enum class EALSMantleType : uint8
{
    HighMantle,
    LowMantle,
    FallingCatch
};

USTRUCT(BlueprintType)
struct FALSComponentAndTransform
{
    GENERATED_BODY()

        UPROPERTY(EditAnywhere, Category = "Character Struct Library")
        FTransform Transform;

    UPROPERTY(EditAnywhere, Category = "Character Struct Library")
        class UPrimitiveComponent* Component = nullptr;
};


USTRUCT(BlueprintType)
struct FALSMantleAsset
{
    GENERATED_BODY()

        UPROPERTY(EditAnywhere, Category = "Mantle System")
        UAnimMontage* AnimMontage = nullptr;

    UPROPERTY(EditAnywhere, Category = "Mantle System")
        UCurveVector* PositionCorrectionCurve = nullptr;

    UPROPERTY(EditAnywhere, Category = "Mantle System")
        FVector StartingOffset;

    UPROPERTY(EditAnywhere, Category = "Mantle System")
        float LowHeight = 0.0f;

    UPROPERTY(EditAnywhere, Category = "Mantle System")
        float LowPlayRate = 0.0f;

    UPROPERTY(EditAnywhere, Category = "Mantle System")
        float LowStartPosition = 0.0f;

    UPROPERTY(EditAnywhere, Category = "Mantle System")
        float HighHeight = 0.0f;

    UPROPERTY(EditAnywhere, Category = "Mantle System")
        float HighPlayRate = 0.0f;

    UPROPERTY(EditAnywhere, Category = "Mantle System")
        float HighStartPosition = 0.0f;
};

UENUM(BlueprintType)
enum class EALSOverlayState : uint8
{
    Default,
    Masculine,
    Feminine,
    Injured,
    HandsTied,
    Rifle,
    PistolOneHanded,
    PistolTwoHanded,
    Bow,
    Torch,
    Binoculars,
    Box,
    Barrel
};

USTRUCT(BlueprintType)
struct FALSMantleParams
{
    GENERATED_BODY()

        UPROPERTY(EditAnywhere, Category = "Mantle System")
        UAnimMontage* AnimMontage = nullptr;

    UPROPERTY(EditAnywhere, Category = "Mantle System")
        UCurveVector* PositionCorrectionCurve = nullptr;

    UPROPERTY(EditAnywhere, Category = "Mantle System")
        float StartingPosition = 0.0f;

    UPROPERTY(EditAnywhere, Category = "Mantle System")
        float PlayRate = 0.0f;

    UPROPERTY(EditAnywhere, Category = "Mantle System")
        FVector StartingOffset;
};

/**
 * Math library functions for ALS
 */
UCLASS()
class UERPGGAME_API UALSMathLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "ALS|Math Utils")
        static FTransform MantleComponentLocalToWorld(const FALSComponentAndTransform& CompAndTransform);

    static TPair<float, float> FixDiagonalGamepadValues(float X, float Y);

    UFUNCTION(BlueprintCallable, Category = "ALS|Math Utils")
        static FTransform TransfromSub(const FTransform& T1, const FTransform& T2)
    {
        return FTransform(T1.GetRotation().Rotator() - T2.GetRotation().Rotator(),
            T1.GetLocation() - T2.GetLocation(), T1.GetScale3D() - T2.GetScale3D());
    }

    UFUNCTION(BlueprintCallable, Category = "ALS|Math Utils")
        static FTransform TransfromAdd(const FTransform& T1, const FTransform& T2)
    {
        return FTransform(T1.GetRotation().Rotator() + T2.GetRotation().Rotator(),
            T1.GetLocation() + T2.GetLocation(), T1.GetScale3D() + T2.GetScale3D());
    }

    UFUNCTION(BlueprintCallable, Category = "ALS|Math Utils")
        static FVector GetCapsuleBaseLocation(float ZOffset, UCapsuleComponent* Capsule);

    UFUNCTION(BlueprintCallable, Category = "ALS|Math Utils")
        static FVector GetCapsuleLocationFromBase(FVector BaseLocation, float ZOffset, UCapsuleComponent* Capsule);

    UFUNCTION(BlueprintCallable, Category = "ALS|Math Utils")
        static bool CapsuleHasRoomCheck(UCapsuleComponent* Capsule, FVector TargetLocation, float HeightOffset,
            float RadiusOffset, EDrawDebugTrace::Type DebugType = EDrawDebugTrace::Type::None, bool DrawDebugTrace = false);

    UFUNCTION(BlueprintCallable, Category = "ALS|Math Utils")
        static bool AngleInRange(float Angle, float MinAngle, float MaxAngle, float Buffer, bool IncreaseBuffer);
};
