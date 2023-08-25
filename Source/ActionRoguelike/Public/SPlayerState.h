// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCreditsChanged, ASPlayerState*, PlayerState, int32, NewCredits, int32, Deltas);

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASPlayerState : public APlayerState
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Credits")
	int32 Credits;

public:
	ASPlayerState();

	UFUNCTION(BlueprintCallable, Category = "Credits")
	void AddCredits(float Deltas);

	UFUNCTION(BlueprintCallable, Category = "Credits")
	bool RemoveCredits(float Deltas);

	UFUNCTION(BlueprintCallable, Category = "Credits")
	int32 GetCredits();

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnCreditsChanged OnCreditsChanged;
};
