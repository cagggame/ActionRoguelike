// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCreditsChanged, ASPlayerState*, PlayerState, int32, NewCredits, int32, Deltas);

class USSaveGame;

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASPlayerState : public APlayerState
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, ReplicatedUsing="OnRep_CreditsChanged", Category = "Credits")
	int32 Credits;

	UFUNCTION()
	void OnRep_CreditsChanged(int32 OldCredits);

public:
	ASPlayerState();

	UFUNCTION(BlueprintCallable, Category = "Credits")
	void AddCredits(int32 Deltas);

	UFUNCTION(BlueprintCallable, Category = "Credits")
	bool RemoveCredits(int32 Deltas);

	UFUNCTION(BlueprintCallable, Category = "Credits")
	int32 GetCredits() const;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnCreditsChanged OnCreditsChanged;

	UFUNCTION(BlueprintNativeEvent)
	void SavePlayerState(USSaveGame* SaveObject);

	UFUNCTION(BlueprintNativeEvent)
	void LoadPlayerState(USSaveGame* SaveObject);
};
