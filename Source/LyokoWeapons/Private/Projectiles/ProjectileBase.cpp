// Copyright © 2025-2027 Lyoko - 96 l'Art Cheperdu
//     __                __       
//    / /   __  ______  / /______ 
//   / /   / / / / __ \/ //_/ __ \
//  / /___/ /_/ / /_/ / ,< / /_/ /
// /_____/ \__, / \____/_/|_| \____/ 
//       /____/                    

#include "Projectiles/ProjectileBase.h"
#include "Projectiles/Properties/ProjectileDamageProperty.h"
#include "Projectiles/Properties/ProjectileRadiusProperty.h"
#include "Projectiles/Properties/ProjectileOnHitBehaviorProperty.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Gameplay/Characters/HealthComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Gameplay/Combat/CombatActor.h"

//----------------------------------------------------------------------------------------------------------------------
bool AProjectileBase::IsProjectileSchemaValid() const
{
    if (!ProjectileSchema) return false;
    return ProjectileSchema->HasProperties(GetMandatoryProperties());
}

//----------------------------------------------------------------------------------------------------------------------
float AProjectileBase::GetDamage() const
{
    if (!ProjectileSchema) return 0.0f;
    const auto DamageProp = ProjectileSchema->GetProperty<UProjectileDamageProperty>();
    return DamageProp ? DamageProp->Damage : 0.0f;
}

//----------------------------------------------------------------------------------------------------------------------
float AProjectileBase::GetRadius() const
{
    if (!ProjectileSchema) return 0.0f;
    const auto RadiusProp = ProjectileSchema->GetProperty<UProjectileRadiusProperty>();
    return RadiusProp ? RadiusProp->Radius : 0.0f;
}

//----------------------------------------------------------------------------------------------------------------------
EProjectileOnHitBehavior AProjectileBase::GetOnHitBehavior() const
{
    if (!ProjectileSchema) return EProjectileOnHitBehavior::Destroy;
    const auto OnHitBehaviorProp = ProjectileSchema->GetProperty<UProjectileOnHitBehaviorProperty>();
    return OnHitBehaviorProp ? OnHitBehaviorProp->OnHitBehavior : EProjectileOnHitBehavior::Destroy;
}

//----------------------------------------------------------------------------------------------------------------------
/**
* Constructor
*/
AProjectileBase::AProjectileBase()
{
    /* Use a sphere as a simple collision representation */
    CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
    CollisionComp->InitSphereRadius(5.0f);
    //CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
    CollisionComp->SetCollisionProfileName(UCollisionProfile::DefaultProjectile_ProfileName);
    //TODO@g: Provide a correct selection between Overlap and Hit.
    //CollisionComp->OnComponentHit.AddDynamic(this, &AProjectileBase::OnHit);
    //CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AProjectileBase::OnBeginOverlap);

    /* Set as root component */
    SetRootComponent(CollisionComp);

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    MeshComponent->SetupAttachment(CollisionComp);

    /* Use a ProjectileMovementComponent to govern this projectile's movement */
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
    ProjectileMovement->UpdatedComponent = CollisionComp;
    ProjectileMovement->InitialSpeed = 3000.f;
    ProjectileMovement->MaxSpeed = 3000.f;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->bShouldBounce = true;
}

//----------------------------------------------------------------------------------------------------------------------
void AProjectileBase::BeginPlay()
{
    Super::BeginPlay();

    CollisionComp->OnComponentHit.AddDynamic(this, &AProjectileBase::OnHit);
    CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AProjectileBase::OnBeginOverlap);
}

//----------------------------------------------------------------------------------------------------------------------
/**
* Called when projectile hits something
* @param HitComp — The component that hit
* @param OtherActor — The actor that was hit
* @param OtherComp — The component that was hit
* @param NormalImpulse — The impulse applied
* @param Hit — The hit result
*/
void AProjectileBase::OnHit_Implementation(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    const bool bHasOwner = IsValid(Owner);
    const bool bHasOwnerOwner = (bHasOwner && IsValid(Owner->GetOwner()));
    const bool bIsOwner = (bHasOwner && OtherActor == Owner);
    const bool bIsOwnerOwner = (bHasOwnerOwner && OtherActor == Owner->GetOwner());
    const bool bHasInstigator = IsValid(GetInstigator());
    const bool bIsInstigator = (bHasInstigator && OtherActor == GetInstigator());
    const bool bIsOtherUnrelated = !bIsOwner && !bIsOwnerOwner && !bIsInstigator;
    if (!bIsOtherUnrelated)
    {
        return;
    }

    OnValidTargetHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);
    ApplyDamages(OtherActor);

    const EProjectileOnHitBehavior OnHitBehavior = GetOnHitBehavior();
    switch (OnHitBehavior)
    {
    case EProjectileOnHitBehavior::Destroy:
    case EProjectileOnHitBehavior::LetTargetDecide:
        Destroy();
        break;
    default:
        break;
    }
}

//----------------------------------------------------------------------------------------------------------------------
void AProjectileBase::OnBeginOverlap_Implementation(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
    OnHit(OverlappedComponent, OtherActor, OtherComp, FVector::ZeroVector, SweepResult);
}

//----------------------------------------------------------------------------------------------------------------------
void AProjectileBase::ApplyDamages(AActor* OtherActor)
{
    if (!OtherActor || OtherActor == this)
        return;

    // Try to get the health component on the hit actor
    UHealthComponent* HealthComp = OtherActor->FindComponentByClass<UHealthComponent>();

    if (HealthComp)
    {
        const float Damage = GetDamage();
        HealthComp->TakeDamage(Damage * BakedDamageMultiplier);
    }
}
