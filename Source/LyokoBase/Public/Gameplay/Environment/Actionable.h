// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Actionable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UActionable : public UInterface
{
	GENERATED_BODY()
};

DECLARE_DYNAMIC_DELEGATE(FOnActivatedDelegate);
DECLARE_DYNAMIC_DELEGATE(FOnDeactivatedDelegate);

/**
 * 
 */
class LYOKOBASE_API IActionable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta = (AutoCreateRefTerm = "Delegate"))
	void BindEventToOnActivated(const FOnActivatedDelegate &Delegate);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta = (AutoCreateRefTerm = "Delegate"))
	void UnbindEventFromOnActivated(const FOnActivatedDelegate &Delegate);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta = (AutoCreateRefTerm = "Delegate"))
	void BindEventToOnDeactivated(const FOnDeactivatedDelegate &Delegate);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta = (AutoCreateRefTerm = "Delegate"))
	void UnbindEventFromOnDeactivated(const FOnDeactivatedDelegate &Delegate);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool GetState();
};