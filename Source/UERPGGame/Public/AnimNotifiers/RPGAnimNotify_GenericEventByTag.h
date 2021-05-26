// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "RPGAnimNotify_GenericEventByTag.generated.h"

/**
 *
 */
UCLASS()
class UERPGGAME_API URPGAnimNotify_GenericEventByTag : public UAnimNotify
{
    GENERATED_BODY()

protected:
    virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FGameplayTag EventTag;
};
