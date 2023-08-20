// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASPlayerState : public APlayerState
{
	GENERATED_BODY()
	
protected:
	int32 Credits;

public:
	ASPlayerState();

	UFUNCTION(BlueprintCallable, Category = "Credits")
	void ApplyCreditsChanged(float Delta);

	UFUNCTION(BlueprintCallable, Category = "Credits")
	int32 GetCredits();
};
