// Copyright © 2025-2027 Lyoko - 96 l'Art Cheperdu
//     __                __       
//    / /   __  ______  / /______ 
//   / /   / / / / __ \/ //_/ __ \
//  /___/ /_/ / /_/ / ,< / /_/ /
// /_____/ \__, / \____/_/|_| \____/ 
//       /____/                    

#include "Projectiles/ImpulseProjectile.h"
#include "Projectiles/Properties/ProjectileImpulseProperty.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

//----------------------------------------------------------------------------------------------------------------------
void AImpulseProjectile::OnHit_Implementation(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    Super::OnHit_Implementation(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);

    const float ImpulseStrength = GetImpulseStrength();
    if (ImpulseStrength > KINDA_SMALL_NUMBER) {
        ApplyImpulse(HitComp, OtherComp, Hit, OtherActor);
    }
}

//----------------------------------------------------------------------------------------------------------------------
void AImpulseProjectile::ApplyImpulse(UPrimitiveComponent* HitComp, UPrimitiveComponent* OtherComp, const FHitResult& Hit, AActor* OtherActor)
{
    const FVector HitVelocity = HitComp->GetComponentVelocity();
    const float ImpulseStrength = GetImpulseStrength();
    const TSubclassOf<AActor> MasterFieldClass = GetMasterFieldClass();

    /* Apply Impulse to surroundings */
    if (OtherComp->IsSimulatingPhysics())
    {
        OtherComp->AddImpulseAtLocation(-Hit.ImpactNormal * ImpulseStrength * HitVelocity.Size(), Hit.ImpactPoint);
    }

    /* Apply Impulse to Character Movement Component */
    UCharacterMovementComponent* OtherCharacterMovementComponent =
        OtherActor->FindComponentByClass<UCharacterMovementComponent>();
    if (OtherCharacterMovementComponent) {
        OtherCharacterMovementComponent->AddImpulse(-Hit.ImpactNormal * ImpulseStrength * HitVelocity.Size());
    }

    /* Spawn Master Field to destroy Geometry Collections */
    if (OtherActor->FindComponentByClass<UGeometryCollectionComponent>() && MasterFieldClass)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        SpawnParams.Instigator = GetInstigator();
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        UWorld* World = GetWorld();
        AActor* MasterField = World->SpawnActor<AActor>(MasterFieldClass, Hit.ImpactPoint, FRotator::ZeroRotator, SpawnParams);
        if (MasterField)
        {
            FTimerHandle TimerHandle;
            World->GetTimerManager().SetTimer(TimerHandle, [MasterField]() { MasterField->Destroy(); }, 0.1f, false);
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------
float AImpulseProjectile::GetImpulseStrength() const
{
    const auto ImpulseProp = GetImpulseProperty();
    return ImpulseProp ? ImpulseProp->ImpulseStrength : 0.0f;
}

//----------------------------------------------------------------------------------------------------------------------
TSubclassOf<AActor> AImpulseProjectile::GetMasterFieldClass() const
{
    const auto ImpulseProp = GetImpulseProperty();
    return ImpulseProp ? ImpulseProp->MasterFieldClass : nullptr;
}
