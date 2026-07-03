// Copyright © 2025 Lyoko - 96 l'Art Cheperdu


#include "Items/RangedWeapon.h"


/**
* Fire a projectile using default weapon settings
*/
void ARangedWeapon::Fire_Implementation()
{
    const FVector OffsetFromMuzzle = FVector(0.f);
    const FTransform MuzzleTransform = GetMuzzleTransform();
    const FVector FireDirection = MuzzleTransform.GetRotation().GetForwardVector();
    const float InitialSpeed = GetProjectileDefaultInitialSpeed();
    Fire(OffsetFromMuzzle, FireDirection, InitialSpeed);
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
    if (ProjectileClass == nullptr) return;

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
    }
}

/**
*/
void ARangedWeapon::FireInDirection_Implementation(const FVector& Direction)
{
    const FVector OffsetFromMuzzle = FVector(0.f);
    Fire(OffsetFromMuzzle, Direction);
}

/**
* Perform an attack with the weapon
*/
void ARangedWeapon::Attack_Implementation()
{
    Fire();
}

/**
* @param Direction - Direction in which to attack
*/
void ARangedWeapon::AttackInDirection_Implementation(const FVector& Direction)
{
    FireInDirection(Direction);
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
    if (ProjectileClass == nullptr) return 0.f;
    AProjectileBase *DefaultProjectileObject = ProjectileClass.GetDefaultObject();
    return DefaultProjectileObject->GetProjectileMovement()->InitialSpeed;
}

/**
*/
float ARangedWeapon::GetProjectileRadius() const
{
    if (ProjectileClass == nullptr) return 0.f;
    AProjectileBase *DefaultProjectileObject = ProjectileClass.GetDefaultObject();
    return DefaultProjectileObject->GetCollisionComp()->GetScaledSphereRadius();
}
