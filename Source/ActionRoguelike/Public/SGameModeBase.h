// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "Engine/DataTable.h"
#include "SGameModeBase.generated.h"

class UEnvQuery;
class UCurveFloat;
class USSaveGame;
class UDataTable;
class USMonsterData;

USTRUCT(BlueprintType)
struct FMonsterInfoRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	FMonsterInfoRow() {
		Weight = 1.0f;
		SpawnCost = 5.0f;
		KillReward = 20.0f;
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FPrimaryAssetId MonsterId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Weight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SpawnCost;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float KillReward;
};

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
	FString SlotName;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UDataTable* MonsterTable;

	UPROPERTY()
	USSaveGame* CurrentSaveGame;

	FTimerHandle TimerHandle_SpawnBots;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TSubclassOf<AActor> MinionClass;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UEnvQuery* SpawnBotQuery;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SpawnTimerInterval;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UCurveFloat* DifficultyCurve;

	UPROPERTY(EditDefaultsOnly, Category = "Credits")
	int32 KillingBonus;

	/* All power-up classes used to spawn with EQS at match start */
	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
	TArray<TSubclassOf<AActor>> PowerupClasses;

	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
	UEnvQuery* SpawnPowerupQuery;

	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
	int32 DesiredPowerupCount;

	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
	float RequiredPowerupDistance;

	void OnMonsterLoaded(FPrimaryAssetId LoadedId, FVector SpawnLocation);

	void SpawnBotsTimerElapsed();

	UFUNCTION()
	void OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION()
	void OnPowerupQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION()
	void RespawnPlayerElapsed(AController* Controller);

public:
	ASGameModeBase();

	virtual void StartPlay() override;

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	virtual void OnActorKilled(AActor* VictimActor, AActor* Killer);

	void OnMinionKilled(AActor* VictimActor, AActor* Killer);

	UFUNCTION(Exec)
	void KillAll();

	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void WriteSaveGame();

	void LoadSaveGame();
};
