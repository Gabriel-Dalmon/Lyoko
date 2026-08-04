// Copyright © 2025-2027 Lyoko - 96 l'Art Cheperdu
//     __                __       
//    / /   __  ______  / /______ 
//   / /   / / / / __ \/ //_/ __ \
//  / /___/ /_/ / /_/ / ,< / /_/ /
// /_____/\__, /\____/_/|_|\____/ 
//       /____/                   

#pragma once

//----------------------------------------------------------------------------------------------------------------------
#include "NativeGameplayTags.h"
#include "Gameplay/Items/ItemRules.h"

//----------------------------------------------------------------------------------------------------------------------
LYOKOWEAPONS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Weapon_OutOfAmmo);
LYOKOWEAPONS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Weapon_Reloading);

//----------------------------------------------------------------------------------------------------------------------
namespace WeaponRules {
    inline const FGameplayTagContainer IsFireDisabledTags = FGameplayTagContainer::CreateFromArray(TArray<FGameplayTag>{
        TAG_Item_Broken,
        TAG_Weapon_OutOfAmmo,
        TAG_Weapon_Reloading
    });

    inline const FGameplayTagContainer IsReloadDisabledTags = FGameplayTagContainer::CreateFromArray(TArray<FGameplayTag>{
        TAG_Item_Broken,
        TAG_Weapon_Reloading
    });
}
