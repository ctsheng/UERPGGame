// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Library/ALSAnimationStructLibrary.h"
#include "RPGAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class UERPGGAME_API URPGAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
    URPGAnimInstance();

    
protected:
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;


public:
    bool GetIsFullBody() { return FullBody; }

	virtual void NativeInitializeAnimation() override;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    bool IsAccelerating;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    bool FullBody;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    bool IsInAir;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    float Speed;

    /** Anim Graph - Foot IK */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	FALSAnimGraphFootIK FootIKValues;

    /** Character Information */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	FALSAnimCharacterInformation CharacterInformation;

    /** References */
	UPROPERTY(BlueprintReadOnly)
	class ARPGCharacter* Character = nullptr;

    void UpdateFootIK(float DeltaSeconds);

    void SetFootLocking(float DeltaSeconds, FName EnableFootIKCurve, FName FootLockCurve, FName IKFootBone,
        float& CurFootLockAlpha, bool& UseFootLockCurve, FVector& CurFootLockLoc, FRotator& CurFootLockRot);

    void SetFootLockOffsets(float DeltaSeconds, FVector& LocalLoc, FRotator& LocalRot);

	void SetPelvisIKOffset(float DeltaSeconds, FVector FootOffsetLTarget,
		FVector FootOffsetRTarget);

	void ResetIKOffsets(float DeltaSeconds);

	void SetFootOffsets(float DeltaSeconds, FName EnableFootIKCurve, FName IKFootBone,
		FName RootBone, FVector& CurLocationTarget, FVector& CurLocationOffset,
		FRotator& CurRotationOffset);
};
