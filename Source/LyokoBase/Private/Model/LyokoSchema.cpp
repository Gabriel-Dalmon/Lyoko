// Copyright © 2025-2027 Lyoko - 96 l'Art Cheperdu


#include "Model/LyokoSchema.h"

#if WITH_EDITOR
//----------------------------------------------------------------------------------------------------------------------
#include "Framework/Notifications/NotificationManager.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "Misc/DataValidation.h" 
#endif

//----------------------------------------------------------------------------------------------------------------------
bool ULyokoSchema::HasProperty(TSubclassOf<ULyokoProperty> PropertyClass) const
{
    return Properties.Contains(PropertyClass);
}

//----------------------------------------------------------------------------------------------------------------------
bool ULyokoSchema::HasProperties(const TSet<TSubclassOf<ULyokoProperty>> &PropertyClasses) const
{
    if (PropertyClasses.Num() > Properties.Num()) [[unlikely]]
    {
        return false;
    }

    for (const TSubclassOf<ULyokoProperty> &PropertyClass : PropertyClasses)
    {
        if (!HasProperty(PropertyClass))
        {
            return false;
        }
    }
    return true;
}

//----------------------------------------------------------------------------------------------------------------------
bool ULyokoSchema::HasAnyProperty(const TSet<TSubclassOf<ULyokoProperty>> &PropertyClasses) const
{
    for (const TSubclassOf<ULyokoProperty> &PropertyClass : PropertyClasses)
    {
        if (HasProperty(PropertyClass))
        {
            return true;
        }
    }
    return false;
}

//----------------------------------------------------------------------------------------------------------------------
ULyokoProperty *ULyokoSchema::GetProperty(TSubclassOf<ULyokoProperty> PropertyClass) const
{
    ensureMsgf(PropertyClass, TEXT("PropertyClass is null. Please provide a valid property class."));
    ensureMsgf(Properties.Contains(PropertyClass), TEXT("PropertyClass %s is not found in Properties map."), *PropertyClass->GetName());
    return Properties.FindRef(PropertyClass);
}

#if WITH_EDITOR
#pragma region Editor
//----------------------------------------------------------------------------------------------------------------------
void ULyokoSchema::PreEditChange(FProperty *PropertyAboutToChange)
{
    Super::PreEditChange(PropertyAboutToChange);
    if (PropertyAboutToChange && PropertyAboutToChange->GetFName() == GET_MEMBER_NAME_CHECKED(ULyokoSchema, Properties))
    {
        PreviousProperties = Properties;
    }
}

//----------------------------------------------------------------------------------------------------------------------
void ULyokoSchema::PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    if (PropertyChangedEvent.Property && PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(ULyokoSchema, Properties))
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
EDataValidationResult ULyokoSchema::IsDataValid(FDataValidationContext &Context) const
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
void ULyokoSchema::OnPropertyKeyChanged(TSubclassOf<ULyokoProperty> &Key, TObjectPtr<ULyokoProperty> &Value)
{
    Value = NewObject<ULyokoProperty>(this, Key);
}

//----------------------------------------------------------------------------------------------------------------------
void ULyokoSchema::OnPropertyValueChanged(TSubclassOf<ULyokoProperty> &Key, TObjectPtr<ULyokoProperty> &Value)
{
    auto NewPropertyClass = Value->GetClass();
    if (PreviousProperties.Contains(NewPropertyClass))
    {
        Value = PreviousProperties[Key];
        PushErrorNotification(NewPropertyClass->GetName() + " is already attached to this data asset. Please choose a different property.");
    }
    else
    {
        Key = NewPropertyClass;
    }
}

//----------------------------------------------------------------------------------------------------------------------
void ULyokoSchema::OnPropertyKeyEmptied(TSubclassOf<ULyokoProperty> &Key, TObjectPtr<ULyokoProperty> &Value)
{
    Value = nullptr;
}

//----------------------------------------------------------------------------------------------------------------------
void ULyokoSchema::OnPropertyValueEmptied(TSubclassOf<ULyokoProperty> &Key, TObjectPtr<ULyokoProperty> &Value)
{
    if (PreviousProperties.Contains(nullptr))
    {
        Value = PreviousProperties[Key];
        PushErrorNotification("There is already an empty property assigned to this data asset.");
    }
    else
    {
        Key = nullptr;
    }
}

//----------------------------------------------------------------------------------------------------------------------
void ULyokoSchema::OnPropertyAdded(TSubclassOf<ULyokoProperty> &Key, TObjectPtr<ULyokoProperty> &Value)
{
}

//----------------------------------------------------------------------------------------------------------------------
void ULyokoSchema::OnPropertyRemoved(TSubclassOf<ULyokoProperty> &Key, TObjectPtr<ULyokoProperty> &Value)
{
}

//----------------------------------------------------------------------------------------------------------------------
void ULyokoSchema::PushErrorNotification(FString Message)
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
