// Copyright © 2025-2027 Lyoko - 96 l'Art Cheperdu


#include "Gameplay/Items/ItemDefinition.h"

#if WITH_EDITOR
//----------------------------------------------------------------------------------------------------------------------
#include "Framework/Notifications/NotificationManager.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "Misc/DataValidation.h" 
#endif

//----------------------------------------------------------------------------------------------------------------------
bool UItemDefinition::HasProperty(TSubclassOf<UItemProperty> PropertyClass) const
{
    return Properties.Contains(PropertyClass);
}

//----------------------------------------------------------------------------------------------------------------------
UItemProperty *UItemDefinition::GetProperty(TSubclassOf<UItemProperty> PropertyClass) const
{
    return Properties.FindRef(PropertyClass);
}

#if WITH_EDITOR
#pragma region Editor
//----------------------------------------------------------------------------------------------------------------------
void UItemDefinition::PreEditChange(FProperty *PropertyAboutToChange)
{
    Super::PreEditChange(PropertyAboutToChange);
    if (PropertyAboutToChange && PropertyAboutToChange->GetFName() == GET_MEMBER_NAME_CHECKED(UItemDefinition, Properties))
    {
        PreviousProperties = Properties;
    }
}

//----------------------------------------------------------------------------------------------------------------------
void UItemDefinition::PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    if (PropertyChangedEvent.Property && PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(UItemDefinition, Properties))
    {
        // Properties Map Size Changed
        if (PreviousProperties.Num() < Properties.Num())
        {
            for (auto &[Key, Value] : Properties)
            {
                if (!PreviousProperties.Contains(Key))
                {
                    OnPropertyAdded(Key, Value);
                }
            }
            return;
        }
        else if (PreviousProperties.Num() > Properties.Num())
        {
            for (auto &[Key, Value] : PreviousProperties)
            {
                if (!Properties.Contains(Key))
                {
                    OnPropertyRemoved(Key, Value);
                    break;
                }
            }
            return;
        }

        // Single Property Changed
        for (auto &[Key, Value] : Properties)
        {
            if (const bool bHasKeyChanged = !PreviousProperties.Contains(Key); bHasKeyChanged)
            {
                if (Key)
                {
                    OnPropertyKeyChanged(Key, Value);

                }
                else
                {
                    OnPropertyKeyEmptied(Key, Value);
                }
            }
            else if (!Key)
            {
                if (Value)
                {
                    OnPropertyValueChanged(Key, Value);
                }
            }
            else if (const bool bHasValueChanged = PreviousProperties[Key] != Value; bHasValueChanged)
            {
                if (Value)
                {
                    OnPropertyValueChanged(Key, Value);
                }
                else
                {
                    OnPropertyValueEmptied(Key, Value);
                }
            }
            
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------
EDataValidationResult UItemDefinition::IsDataValid(FDataValidationContext &Context) const
{
    EDataValidationResult Result = EDataValidationResult::Valid;

    for (auto &[Key, Value] : Properties)
    {
        if (!Value)
        {
            Context.AddError(FText::FromString("Properties map still contains null property."));
            Result = EDataValidationResult::Invalid;
        }
        else if (Key != Value->GetClass())
        {
            Context.AddError(FText::FromString("Properties map contains mismatched key and value."));
            Result = EDataValidationResult::Invalid;
        }
        else if (Value->IsDataValid(Context) == EDataValidationResult::Invalid)
        {
            Result = EDataValidationResult::Invalid;
        }
    }
    return Result;
}

//----------------------------------------------------------------------------------------------------------------------
void UItemDefinition::OnPropertyKeyChanged(TSubclassOf<UItemProperty> &Key, TObjectPtr<UItemProperty> &Value)
{
    Value = NewObject<UItemProperty>(this, Key);
}

//----------------------------------------------------------------------------------------------------------------------
void UItemDefinition::OnPropertyValueChanged(TSubclassOf<UItemProperty> &Key, TObjectPtr<UItemProperty> &Value)
{
    auto NewPropertyClass = Value->GetClass();
    if (PreviousProperties.Contains(NewPropertyClass))
    {
        Value = PreviousProperties[Key];
        PushErrorNotification(NewPropertyClass->GetName() + " is already attached to this item. Please choose a different property.");
    }
    else
    {
        Key = NewPropertyClass;
    }
}

//----------------------------------------------------------------------------------------------------------------------
void UItemDefinition::OnPropertyKeyEmptied(TSubclassOf<UItemProperty> &Key, TObjectPtr<UItemProperty> &Value)
{
    Value = nullptr;
}

//----------------------------------------------------------------------------------------------------------------------
void UItemDefinition::OnPropertyValueEmptied(TSubclassOf<UItemProperty> &Key, TObjectPtr<UItemProperty> &Value)
{
    if (PreviousProperties.Contains(nullptr))
    {
        Value = PreviousProperties[Key];
        PushErrorNotification("There is already an empty property assigned to this item.");
    }
    else
    {
        Key = nullptr;
    }
}

//----------------------------------------------------------------------------------------------------------------------
void UItemDefinition::OnPropertyAdded(TSubclassOf<UItemProperty> &Key, TObjectPtr<UItemProperty> &Value)
{
}

//----------------------------------------------------------------------------------------------------------------------
void UItemDefinition::OnPropertyRemoved(TSubclassOf<UItemProperty> &Key, TObjectPtr<UItemProperty> &Value)
{
}

//----------------------------------------------------------------------------------------------------------------------
void UItemDefinition::PushErrorNotification(FString Message)
{
    FNotificationInfo Info(
        FText::FromString(Message)
    );
    Info.ExpireDuration = 3.0f;
    TSharedPtr<SNotificationItem> Notification =
        FSlateNotificationManager::Get().AddNotification(Info);
    if (Notification.IsValid())
    {
        Notification->SetCompletionState(
            SNotificationItem::CS_Fail
        );
    }
}
#pragma endregion Editor
#endif