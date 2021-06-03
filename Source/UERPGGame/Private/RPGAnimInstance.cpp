// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Characters/RPGCharacter.h"

const FName NAME_BasePose_CLF(TEXT("BasePose_CLF"));
const FName NAME_BasePose_N(TEXT("BasePose_N"));
const FName NAME_Enable_FootIK_R(TEXT("Enable_FootIK_R"));
const FName NAME_Enable_FootIK_L(TEXT("Enable_FootIK_L"));
const FName NAME_Enable_HandIK_L(TEXT("Enable_HandIK_L"));
const FName NAME_Enable_HandIK_R(TEXT("Enable_HandIK_R"));
const FName NAME_Enable_Transition(TEXT("Enable_Transition"));
const FName NAME_FootLock_L(TEXT("FootLock_L"));
const FName NAME_FootLock_R(TEXT("FootLock_R"));
const FName NAME_Grounded___Slot(TEXT("Grounded Slot"));
const FName NAME_Layering_Arm_L(TEXT("Layering_Arm_L"));
const FName NAME_Layering_Arm_L_Add(TEXT("Layering_Arm_L_Add"));
const FName NAME_Layering_Arm_L_LS(TEXT("Layering_Arm_L_LS"));
const FName NAME_Layering_Arm_R(TEXT("Layering_Arm_R"));
const FName NAME_Layering_Arm_R_Add(TEXT("Layering_Arm_R_Add"));
const FName NAME_Layering_Arm_R_LS(TEXT("Layering_Arm_R_LS"));
const FName NAME_Layering_Hand_L(TEXT("Layering_Hand_L"));
const FName NAME_Layering_Hand_R(TEXT("Layering_Hand_R"));
const FName NAME_Layering_Head_Add(TEXT("Layering_Head_Add"));
const FName NAME_Layering_Spine_Add(TEXT("Layering_Spine_Add"));
const FName NAME_Mask_AimOffset(TEXT("Mask_AimOffset"));
const FName NAME_Mask_LandPrediction(TEXT("Mask_LandPrediction"));
const FName NAME__ALSCharacterAnimInstance__RotationAmount(TEXT("RotationAmount"));
const FName NAME_VB___foot_target_l(TEXT("VB foot_target_l"));
const FName NAME_VB___foot_target_r(TEXT("VB foot_target_r"));
const FName NAME_W_Gait(TEXT("W_Gait"));
const FName NAME__ALSCharacterAnimInstance__root(TEXT("root"));

const FName NAME_ik_foot_l(TEXT("ik_foot_l"));
const FName NAME_ik_foot_r(TEXT("ik_foot_r"));

const float FootHeight = 13.5f;
const float IK_TraceDistanceAboveFoot = 50.0f;
const float IK_TraceDistanceBelowFoot = 45.0f;

URPGAnimInstance::URPGAnimInstance()
	: IsAccelerating(false), FullBody(false), IsInAir(false), Speed(0.0f)
{
}


void URPGAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	Character = Cast<ARPGCharacter>(TryGetPawnOwner());
}


void URPGAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{

	if (!Character)
		return;

	UCharacterMovementComponent* Movement = Cast<UCharacterMovementComponent>(Character->GetMovementComponent());

	if (Movement)
	{
		IsInAir = Movement->IsFalling();
		IsAccelerating = Movement->GetCurrentAcceleration().Size() > 0;
	}

	FullBody = GetCurveValue("FullBody") > 0;
	Speed = Character->GetVelocity().Size();


	// Update rest of character information. Others are reflected into anim bp when they're set inside character class
	CharacterInformation.Velocity = Character->GetCharacterMovement()->Velocity;
	CharacterInformation.CharacterActorRotation = Character->GetActorRotation();

	UpdateFootIK(DeltaSeconds);
}


void URPGAnimInstance::UpdateFootIK(float DeltaSeconds)
{
	FVector FootOffsetLTarget = FVector::ZeroVector;
	FVector FootOffsetRTarget = FVector::ZeroVector;

	// Update Foot Locking values.
	SetFootLocking(DeltaSeconds, NAME_Enable_FootIK_L, NAME_FootLock_L,
		NAME_ik_foot_l, FootIKValues.FootLock_L_Alpha, FootIKValues.UseFootLockCurve_L,
		FootIKValues.FootLock_L_Location, FootIKValues.FootLock_L_Rotation);
	SetFootLocking(DeltaSeconds, NAME_Enable_FootIK_R, NAME_FootLock_R,
		NAME_ik_foot_r, FootIKValues.FootLock_R_Alpha, FootIKValues.UseFootLockCurve_R,
		FootIKValues.FootLock_R_Location, FootIKValues.FootLock_R_Rotation);

	if (IsInAir)
	{
		// Reset IK Offsets if In Air
		SetPelvisIKOffset(DeltaSeconds, FVector::ZeroVector, FVector::ZeroVector);
		ResetIKOffsets(DeltaSeconds);
	}
	else
	{
		// Update all Foot Lock and Foot Offset values when not In Air
		SetFootOffsets(DeltaSeconds, NAME_Enable_FootIK_L, NAME_ik_foot_l, NAME__ALSCharacterAnimInstance__root,
			FootOffsetLTarget,
			FootIKValues.FootOffset_L_Location, FootIKValues.FootOffset_L_Rotation);
		SetFootOffsets(DeltaSeconds, NAME_Enable_FootIK_R, NAME_ik_foot_r, NAME__ALSCharacterAnimInstance__root,
			FootOffsetRTarget,
			FootIKValues.FootOffset_R_Location, FootIKValues.FootOffset_R_Rotation);
		SetPelvisIKOffset(DeltaSeconds, FootOffsetLTarget, FootOffsetRTarget);
	}
}


void URPGAnimInstance::SetFootLocking(float DeltaSeconds, FName EnableFootIKCurve, FName FootLockCurve,
	FName IKFootBone, float& CurFootLockAlpha, bool& UseFootLockCurve,
	FVector& CurFootLockLoc, FRotator& CurFootLockRot)
{
	if (GetCurveValue(EnableFootIKCurve) <= 0.0f)
	{
		return;
	}

	// Step 1: Set Local FootLock Curve value
	float FootLockCurveVal;

	if (UseFootLockCurve)
	{
		UseFootLockCurve = FMath::Abs(GetCurveValue(NAME__ALSCharacterAnimInstance__RotationAmount)) <= 0.001f ||
			Character->GetLocalRole() != ROLE_AutonomousProxy;
		FootLockCurveVal = GetCurveValue(FootLockCurve) * (1.f / GetSkelMeshComponent()->AnimUpdateRateParams->UpdateRate);
	}
	else
	{
		UseFootLockCurve = GetCurveValue(FootLockCurve) >= 0.99f;
		FootLockCurveVal = 0.0f;
	}

	// Step 2: Only update the FootLock Alpha if the new value is less than the current, or it equals 1. This makes it
	// so that the foot can only blend out of the locked position or lock to a new position, and never blend in.
	if (FootLockCurveVal >= 0.99f || FootLockCurveVal < CurFootLockAlpha)
	{
		CurFootLockAlpha = FootLockCurveVal;
	}

	// Step 3: If the Foot Lock curve equals 1, save the new lock location and rotation in component space as the target.
	if (CurFootLockAlpha >= 0.99f)
	{
		const FTransform& OwnerTransform =
			GetOwningComponent()->GetSocketTransform(IKFootBone, RTS_Component);
		CurFootLockLoc = OwnerTransform.GetLocation();
		CurFootLockRot = OwnerTransform.Rotator();
	}

	// Step 4: If the Foot Lock Alpha has a weight,
	// update the Foot Lock offsets to keep the foot planted in place while the capsule moves.
	if (CurFootLockAlpha > 0.0f)
	{
		SetFootLockOffsets(DeltaSeconds, CurFootLockLoc, CurFootLockRot);
	}
}


void URPGAnimInstance::SetFootLockOffsets(float DeltaSeconds, FVector& LocalLoc, FRotator& LocalRot)
{
	FRotator RotationDifference = FRotator::ZeroRotator;
	// Use the delta between the current and last updated rotation to find how much the foot should be rotated
	// to remain planted on the ground.
	if (Character->GetCharacterMovement()->IsMovingOnGround())
	{
		RotationDifference = CharacterInformation.CharacterActorRotation - Character->GetCharacterMovement()->
			GetLastUpdateRotation();
		RotationDifference.Normalize();
	}

	// Get the distance traveled between frames relative to the mesh rotation
	// to find how much the foot should be offset to remain planted on the ground.
	const FVector& LocationDifference = GetOwningComponent()->GetComponentRotation().UnrotateVector(
		CharacterInformation.Velocity * DeltaSeconds);

	// Subtract the location difference from the current local location and rotate
	// it by the rotation difference to keep the foot planted in component space.
	LocalLoc = (LocalLoc - LocationDifference).RotateAngleAxis(RotationDifference.Yaw, FVector::DownVector);

	// Subtract the Rotation Difference from the current Local Rotation to get the new local rotation.
	FRotator Delta = LocalRot - RotationDifference;
	Delta.Normalize();
	LocalRot = Delta;
}


void URPGAnimInstance::SetPelvisIKOffset(float DeltaSeconds, FVector FootOffsetLTarget,
	FVector FootOffsetRTarget)
{
	// Calculate the Pelvis Alpha by finding the average Foot IK weight. If the alpha is 0, clear the offset.
	FootIKValues.PelvisAlpha =
		(GetCurveValue(NAME_Enable_FootIK_L) + GetCurveValue(NAME_Enable_FootIK_R)) / 2.0f;

	if (FootIKValues.PelvisAlpha > 0.0f)
	{
		// Step 1: Set the new Pelvis Target to be the lowest Foot Offset
		const FVector PelvisTarget = FootOffsetLTarget.Z < FootOffsetRTarget.Z ? FootOffsetLTarget : FootOffsetRTarget;

		// Step 2: Interp the Current Pelvis Offset to the new target value.
		//Interpolate at different speeds based on whether the new target is above or below the current one.
		const float InterpSpeed = PelvisTarget.Z > FootIKValues.PelvisOffset.Z ? 10.0f : 15.0f;
		FootIKValues.PelvisOffset =
			FMath::VInterpTo(FootIKValues.PelvisOffset, PelvisTarget, DeltaSeconds, InterpSpeed);
	}
	else
	{
		FootIKValues.PelvisOffset = FVector::ZeroVector;
	}
}


void URPGAnimInstance::ResetIKOffsets(float DeltaSeconds)
{
	// Interp Foot IK offsets back to 0
	FootIKValues.FootOffset_L_Location = FMath::VInterpTo(FootIKValues.FootOffset_L_Location,
		FVector::ZeroVector, DeltaSeconds, 15.0f);
	FootIKValues.FootOffset_R_Location = FMath::VInterpTo(FootIKValues.FootOffset_R_Location,
		FVector::ZeroVector, DeltaSeconds, 15.0f);
	FootIKValues.FootOffset_L_Rotation = FMath::RInterpTo(FootIKValues.FootOffset_L_Rotation,
		FRotator::ZeroRotator, DeltaSeconds, 15.0f);
	FootIKValues.FootOffset_R_Rotation = FMath::RInterpTo(FootIKValues.FootOffset_R_Rotation,
		FRotator::ZeroRotator, DeltaSeconds, 15.0f);
}


void URPGAnimInstance::SetFootOffsets(float DeltaSeconds, FName EnableFootIKCurve, FName IKFootBone,
	FName RootBone, FVector& CurLocationTarget, FVector& CurLocationOffset,
	FRotator& CurRotationOffset)
{
	// Only update Foot IK offset values if the Foot IK curve has a weight. If it equals 0, clear the offset values.
	if (GetCurveValue(EnableFootIKCurve) <= 0)
	{
		CurLocationOffset = FVector::ZeroVector;
		CurRotationOffset = FRotator::ZeroRotator;
		return;
	}

	// Step 1: Trace downward from the foot location to find the geometry.
	// If the surface is walkable, save the Impact Location and Normal.
	USkeletalMeshComponent* OwnerComp = GetOwningComponent();
	FVector IKFootFloorLoc = OwnerComp->GetSocketLocation(IKFootBone);
	IKFootFloorLoc.Z = OwnerComp->GetSocketLocation(RootBone).Z;

	UWorld* World = GetWorld();
	check(World);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Character);

	const FVector TraceStart = IKFootFloorLoc + FVector(0.0, 0.0, IK_TraceDistanceAboveFoot);
	const FVector TraceEnd = IKFootFloorLoc - FVector(0.0, 0.0, IK_TraceDistanceBelowFoot);

	FHitResult HitResult;
	const bool bHit = World->LineTraceSingleByChannel(HitResult,
		TraceStart,
		TraceEnd,
		ECC_Visibility, Params);

	FRotator TargetRotOffset = FRotator::ZeroRotator;
	if (Character->GetCharacterMovement()->IsWalkable(HitResult))
	{
		FVector ImpactPoint = HitResult.ImpactPoint;
		FVector ImpactNormal = HitResult.ImpactNormal;

		// Step 1.1: Find the difference in location from the Impact point and the expected (flat) floor location.
		// These values are offset by the nomrmal multiplied by the
		// foot height to get better behavior on angled surfaces.
		CurLocationTarget = (ImpactPoint + ImpactNormal * FootHeight) -
			(IKFootFloorLoc + FVector(0, 0, FootHeight));

		// Step 1.2: Calculate the Rotation offset by getting the Atan2 of the Impact Normal.
		TargetRotOffset.Pitch = -FMath::RadiansToDegrees(FMath::Atan2(ImpactNormal.X, ImpactNormal.Z));
		TargetRotOffset.Roll = FMath::RadiansToDegrees(FMath::Atan2(ImpactNormal.Y, ImpactNormal.Z));
	}

	// Step 2: Interp the Current Location Offset to the new target value.
	// Interpolate at different speeds based on whether the new target is above or below the current one.
	const float InterpSpeed = CurLocationOffset.Z > CurLocationTarget.Z ? 30.f : 15.0f;
	CurLocationOffset = FMath::VInterpTo(CurLocationOffset, CurLocationTarget, DeltaSeconds, InterpSpeed);

	// Step 3: Interp the Current Rotation Offset to the new target value.
	CurRotationOffset = FMath::RInterpTo(CurRotationOffset, TargetRotOffset, DeltaSeconds, 30.0f);
}
