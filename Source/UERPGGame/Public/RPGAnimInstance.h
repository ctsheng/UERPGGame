// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
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

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    bool IsAccelerating;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    bool FullBody;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    bool IsInAir;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    float Speed;
};
