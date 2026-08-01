// Copyright � 2025 Lyoko - 96 l'Art Cheperdu


#include "Items/RangedWeapon.h"
#include "Items/RangedWeaponData.h"
#include "Items/WeaponRules.h"

void ARangedWeapon::OnSecondaryInteracted_Implementation()
{
    Reload();
}

/**
* @param Direction - Direction in which to attack
*/
void ARangedWeapon::AttackInDirection_Implementation(const FVector &Direction)
{
    FireInDirection(Direction);
}

//----------------------------------------------------------------------------------------------------------------------
bool ARangedWeapon::IsAttackAvailable_Implementation() const
{
    return ItemTags.HasAny(WeaponRules::IsFireDisabledTags) && Super::IsAttackAvailable_Implementation();
}

//----------------------------------------------------------------------------------------------------------------------
FVector ARangedWeapon::GetDefaultAttackDirection_Implementation() const
{
    return GetMuzzleTransform(ERelativeTransformSpace::RTS_World).GetRotation().GetForwardVector();
}

//----------------------------------------------------------------------------------------------------------------------
void ARangedWeapon::OnAmmunitionEmpty_Implementation()
{
    ItemTags.AddTag(TAG_Weapon_OutOfAmmo);
    if (bAutomaticReload)
    {
        Reload();
    }
    OnAmmunitionEmptyEvent.Broadcast();
}

//----------------------------------------------------------------------------------------------------------------------
bool ARangedWeapon::IsFireAvailable_Implementation() const
{
    return WeaponRules::IsFireDisabledTags.IsEmpty() || !ItemTags.HasAny(WeaponRules::IsFireDisabledTags);
}

/**
* Fire a projectile using custom settings
* @param OffsetFromMuzzle - Offset from the muzzle location
* @param FireDirection - Direction in which the projectile will be fired
*/
void ARangedWeapon::Fire(const FVector &OffsetFromMuzzle, const FVector &Direction)
{
    const float InitialSpeed = GetProjectileDefaultInitialSpeed();
    Fire(OffsetFromMuzzle, Direction, InitialSpeed);
}

/**
* Fire a projectile using custom settings
* @param OffsetFromMuzzle - Offset from the muzzle location
* @param FireDirection - Direction in which the projectile will be fired
* @param InitialSpeed - Initial speed of the projectile
*/
void ARangedWeapon::Fire(const FVector &OffsetFromMuzzle, const FVector & Direction, const float InitialSpeed)
{
    auto WeaponData = GetRangedWeaponData();
    TSubclassOf<AProjectileBase> ProjectileClass = WeaponData->ProjectileClass;

    const FTransform MuzzleTransform = GetMuzzleTransform();
    const FVector MuzzleLocation = MuzzleTransform.GetLocation();
    const FQuat MuzzleRotation = MuzzleTransform.GetRotation();

    const FVector SpawnLocation = MuzzleTransform.GetLocation() + MuzzleRotation.RotateVector(OffsetFromMuzzle);

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    SpawnParams.Instigator = GetInstigator();
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    auto World = GetWorld();
    check(GetWorld());
    check(!SpawnLocation.ContainsNaN());
    check(!MuzzleRotation.ContainsNaN());
    //TODO@g: Spawn deferred and apply Velocity and DamageMultiplier before spawning.
    AProjectileBase *Projectile = World->SpawnActor<AProjectileBase>(
        ProjectileClass, 
        SpawnLocation, 
        MuzzleRotation.Rotator(), 
        SpawnParams
    );

    UE_CLOG(!Projectile, LogTemp, Error, TEXT("%s: Failed to spawn projectile of class %s!"), *ARangedWeapon::StaticClass()->GetName(), *ProjectileClass->GetName());

    if (Projectile)
    {
        const FVector InitialVelocity = Direction * InitialSpeed;
        Projectile->GetProjectileMovement()->Velocity = InitialVelocity;
        Projectile->BakedDamageMultiplier = ComputeDamageMultiplier_Implementation();
        --CurrentAmmunitionCount;

        if (CurrentAmmunitionCount <= 0)
        {
            OnAmmunitionEmpty();
        }
        OnFired(Projectile);
    }
}

//----------------------------------------------------------------------------------------------------------------------
void ARangedWeapon::FireInDirection_Implementation(const FVector& Direction)
{
    const FVector OffsetFromMuzzle = FVector(0.f);
    Fire(OffsetFromMuzzle, Direction);
}

void ARangedWeapon::OnFired_Implementation(AProjectileBase *Projectile)
{
}

//----------------------------------------------------------------------------------------------------------------------
float ARangedWeapon::ComputeDamageMultiplier_Implementation() const
{
    float BaseMultiplier = Super::ComputeDamageMultiplier_Implementation();
    auto WeaponData = GetRangedWeaponData();
    if (!WeaponData) [[unlikely]] return BaseMultiplier;

    return BaseMultiplier * WeaponData->DamageModifier;
}



//----------------------------------------------------------------------------------------------------------------------
bool ARangedWeapon::IsReloading() const
{
    return ItemTags.HasTag(TAG_Weapon_Reloading);
}

//----------------------------------------------------------------------------------------------------------------------
bool ARangedWeapon::IsReloadAvailable_Implementation() const
{
    return WeaponRules::IsReloadDisabledTags.IsEmpty() || !ItemTags.HasAny(WeaponRules::IsReloadDisabledTags);
}

//----------------------------------------------------------------------------------------------------------------------
void ARangedWeapon::Reload()
{
    auto WeaponData = GetRangedWeaponData();
    switch (WeaponData->ReloadPattern)
    {
    case EReloadPattern::Complete:
        ReloadMax();
        break;
    case EReloadPattern::Incremental:
        ReloadBatch(WeaponData->ReloadBatchSize);
        break;
    default:
        break;
    }
}

//----------------------------------------------------------------------------------------------------------------------
void ARangedWeapon::ReloadMax()
{
    ReloadBatch(GetRangedWeaponData()->Capacity);
}

//----------------------------------------------------------------------------------------------------------------------
void ARangedWeapon::ReloadBatch(int BatchSize)
{
    if (!IsReloadAvailable()) return;
    ensureMsgf(BatchSize > 0, TEXT("%s: Invalid batch size %d!"), *ARangedWeapon::StaticClass()->GetName(), BatchSize);

    auto WeaponData = GetRangedWeaponData();
    const float ReloadDuration = WeaponData->ReloadDuration;
    const int SafeBatchSize = FMath::Clamp(BatchSize, 1, WeaponData->Capacity - CurrentAmmunitionCount);

    ItemTags.AddTag(TAG_Weapon_Reloading);
    OnReloadStarted();

    GetWorldTimerManager().SetTimer(ReloadTimerHandle, [this, ReloadDuration, SafeBatchSize]()
        {
            ItemTags.RemoveTag(TAG_Weapon_Reloading);
            ItemTags.RemoveTag(TAG_Weapon_OutOfAmmo);
            CurrentAmmunitionCount = SafeBatchSize;
            OnReloaded(SafeBatchSize);
        }, ReloadDuration, false);
}

//----------------------------------------------------------------------------------------------------------------------
void ARangedWeapon::CancelReload()
{
    if (!IsReloading()) return;
    GetWorldTimerManager().ClearTimer(ReloadTimerHandle);
    ItemTags.RemoveTag(TAG_Weapon_Reloading);
    OnReloadCanceled();
}

//----------------------------------------------------------------------------------------------------------------------
void ARangedWeapon::OnReloadStarted_Implementation()
{
}

//----------------------------------------------------------------------------------------------------------------------
void ARangedWeapon::OnReloaded_Implementation(int BatchSize)
{
    OnReloadedEvent.Broadcast(BatchSize);
}

//----------------------------------------------------------------------------------------------------------------------
void ARangedWeapon::OnReloadCanceled_Implementation()
{
}

/**
* Get the muzzle transform subobject
* @param TransformSpace - Space in which the transform should be returned
* @return Muzzle transform subobject
*/
FTransform ARangedWeapon::GetMuzzleTransform(ERelativeTransformSpace TransformSpace) const
{
    checkf(MeshComponent && MeshComponent->DoesSocketExist(MuzzleSocketName),
        TEXT("%s: Socket '%s' not found on mesh!"), *ARangedWeapon::StaticClass()->GetName(), *MuzzleSocketName.ToString());

    return MeshComponent->GetSocketTransform(MuzzleSocketName, TransformSpace);
}

/**
* Get the default initial speed of the projectile
* @return Initial speed of the projectile
*/
float ARangedWeapon::GetProjectileDefaultInitialSpeed() const
{
    auto WeaponData = GetRangedWeaponData();
    TSubclassOf<AProjectileBase> ProjectileClass = WeaponData->ProjectileClass;

    AProjectileBase *DefaultProjectileObject = ProjectileClass.GetDefaultObject();
    return DefaultProjectileObject->GetProjectileMovement()->InitialSpeed;
}

/**
*/
float ARangedWeapon::GetProjectileRadius() const
{
    auto WeaponData = GetRangedWeaponData();
    TSubclassOf<AProjectileBase> ProjectileClass = WeaponData->ProjectileClass;

    AProjectileBase *DefaultProjectileObject = ProjectileClass.GetDefaultObject();
    return DefaultProjectileObject->GetCollisionComp()->GetScaledSphereRadius();
}

