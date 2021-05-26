// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PlayerCharacter.h"
#include "Math/NumericLimits.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Items/RPGWeaponItem.h"
#include "Items/WeaponBase.h"
#include "RPGGameInstance.h"
#include "AbilitySystem/RPGAbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "Player/RPGPlayerState.h"
#include "Characters/RPGAICharacter.h"
#include "Components/ALSMantleComponent.h"
#include "RPGAnimInstance.h"
#include "Kismet/GameplayStatics.h"


APlayerCharacter::APlayerCharacter(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    IsEnabledComboPeriod = false;
    AttackCount = 0;

    bUseControllerRotationPitch = false;
    bUseControllerRotationRoll = false;
    bUseControllerRotationYaw = false;

    GetCharacterMovement()->bOrientRotationToMovement = true;

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 300.0f;
    CameraBoom->bUsePawnControlRotation = true;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Follow Camera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;

    MantleComponent = CreateDefaultSubobject<UALSMantleComponent>(TEXT("Mantle"));
}


// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

    UWorld* World = GetWorld();
    URPGGameInstance* GameInstance = World ? World->GetGameInstance<URPGGameInstance>() : nullptr;

    if (!GameInstance)
    {
        return;
    }

    URPGAssetManager& AssetManager = URPGAssetManager::Get();
    URPGItem* LoadedItem = AssetManager.ForceLoadItem(GameInstance->DefaultWeapon);
    URPGWeaponItem* InitialWeapon = Cast<URPGWeaponItem>(LoadedItem);

    if (InitialWeapon)
    {
        FActorSpawnParameters SpawnParams;

        if (HasAuthority())
        {
            SpawnParams.Owner = this;
            SpawnParams.Instigator = this;

            CurrentWeapon = World->SpawnActor<AWeaponBase>(InitialWeapon->WeaponActor, SpawnParams);
            CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("Sword"));
        }
    }
}


// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}


void APlayerCharacter::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

    InitPlayerAbilitySystem();
    AddStartupEffects();
}


void APlayerCharacter::OnRep_PlayerState()
{
    Super::OnRep_PlayerState();

    InitPlayerAbilitySystem();
}


void APlayerCharacter::InitPlayerAbilitySystem()
{
    ARPGPlayerState* PS = GetPlayerState<ARPGPlayerState>();
    if (PS)
    {
        // Set the ASC for clients. Server does this in PossessedBy.
        AbilitySystemComponent = Cast<URPGAbilitySystemComponent>(PS->GetAbilitySystemComponent());

        // Init ASC Actor Info for clients. Server will init its ASC when it possesses a new Actor.
        AbilitySystemComponent->InitAbilityActorInfo(PS, this);

        // Set the AttributeSetBase for convenience attribute functions
        AttributeSet = PS->GetAttributeSetBase();

        AddStartupGameplayAbilities();
    }
}


void APlayerCharacter::AddStartupGameplayAbilities()
{
    if (!AbilitySystemComponent.IsValid())
    {
        return;
    }

    if (GetLocalRole() == ROLE_Authority)
    {
        for (auto &GA : GAPrimaryAttacks)
            AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(GA));

        if (GADash)
            AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(GADash));

        if (GASprint)
            AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(GASprint));

        if (GASkill)
            AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(GASkill));
    }
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

    PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);

    PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &APlayerCharacter::JumpPressedAction);

    PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &APlayerCharacter::DoPrimaryAttack);
    PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &APlayerCharacter::Dash);
    PlayerInputComponent->BindAction("Skill", IE_Pressed, this, &APlayerCharacter::DoSkill);
}

class USpringArmComponent* APlayerCharacter::GetCameraBoom()
{
    return CameraBoom;
}

class UCameraComponent* APlayerCharacter::GetFollowCamera()
{
    return FollowCamera;
}

void APlayerCharacter::MoveForward(float Value)
{
    if (GetIsFullBodyAnimating())
    {
        return;
    }

    if ((Controller != nullptr) && (Value != 0.0f))
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        AddMovementInput(Direction, Value);
    }
}


void APlayerCharacter::MoveRight(float Value)
{
    if (GetIsFullBodyAnimating())
    {
        return;
    }

    if ((Controller != nullptr) && (Value != 0.0f))
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
        AddMovementInput(Direction, Value);
    }
}


void APlayerCharacter::JumpPressedAction()
{
    // todo ÅÊÅÀ
    // MantleComponent->OnOwnerJumpInput();
    Super::Jump();

    if (AbilitySystemComponent.IsValid())
    {
        FGameplayTagContainer AbilityTag;
        AbilityTag.AddTag(FGameplayTag::RequestGameplayTag("Ability.Sprint"));
        AbilitySystemComponent->CancelAbilities(&AbilityTag);
    }
}


void APlayerCharacter::Dash()
{
    if (AbilitySystemComponent.IsValid())
    {
        AbilitySystemComponent->TryActivateAbilityByClass(GADash);
    }
}

void APlayerCharacter::DoSkill()
{
    if (AbilitySystemComponent.IsValid())
    {
        AbilitySystemComponent->TryActivateAbilityByClass(GASkill);
    }
}

void APlayerCharacter::Sprint()
{
    if (AbilitySystemComponent.IsValid())
    {
        AbilitySystemComponent->TryActivateAbilityByClass(GASprint);
    }
}


void APlayerCharacter::DoPrimaryAttack()
{
    if (IsUsingMelee())
    {
        if (IsEnabledComboPeriod)
        {
            UAnimInstance* AnimInst = GetMesh()->GetAnimInstance();
            UAnimMontage* Montage = AnimInst->GetCurrentActiveMontage();

            if (AnimInst)
            {
                TSubclassOf<URPGAbility_MeleeBase> PrimaryAttack = GetPrimaryAttackCombo(++AttackCount);
                if (PrimaryAttack)
                {
                    AbilitySystemComponent->TryActivateAbilityByClass(PrimaryAttack);
                }
                IsEnabledComboPeriod = false;
            }
        }
    }
    else
    {
        AttackCount = 0;
        TSubclassOf<URPGAbility_MeleeBase> PrimaryAttack = GetPrimaryAttackCombo(AttackCount);

        if (PrimaryAttack)
        {
            AbilitySystemComponent->TryActivateAbilityByClass(PrimaryAttack);
        }
    }
}


bool APlayerCharacter::GetIsFullBodyAnimating()
{
    USkeletalMeshComponent* MeshComponent = GetMesh();
    if (MeshComponent)
    {
        URPGAnimInstance* AnimInst = Cast<URPGAnimInstance>(MeshComponent->GetAnimInstance());
        if (AnimInst)
        {
            return AnimInst->GetIsFullBody();
        }
    }
    return false;
}


TSubclassOf<URPGAbility_MeleeBase> APlayerCharacter::GetPrimaryAttackCombo(int Count)
{
    if (Count < GAPrimaryAttacks.Num())
    {
        return GAPrimaryAttacks[Count];
    }
    return NULL;
}


bool APlayerCharacter::IsUsingMelee()
{
    if (AbilitySystemComponent.IsValid())
    {
        FGameplayTagContainer AbilityTag;
        AbilityTag.AddTag(FGameplayTag::RequestGameplayTag("Ability.Melee"));
        TArray<URPGGameplayAbility*> AbilitiesArray;
        AbilitySystemComponent->GetActiveAbilitiesWithTags(AbilityTag, AbilitiesArray);
        for (auto& Ability : AbilitiesArray)
        {
            if (Ability->IsActive())
            {
                return true;
            }
        }
    }

    return false;
}

AActor* APlayerCharacter::TryGetNearestEnemy()
{
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARPGAICharacter::StaticClass(), FoundActors);

    AActor* NearestOne = nullptr;
    float NearestDistance = TNumericLimits<float>::Max();
    float AttackRange = 1500;
    for (auto Minion: FoundActors)
    {
        auto Dis = FVector::Distance(GetActorLocation(), Minion->GetActorLocation());
        if (Dis < AttackRange && NearestDistance > Dis)
        {
            NearestDistance = Dis;
            NearestOne = Minion;
        }
    }

    return NearestOne;
}

