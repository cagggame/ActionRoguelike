// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "AI/SAICharacter.h"
#include "SAttributeComponent.h"
#include "EngineUtils.h"
#include "DrawDebugHelpers.h"
#include "SCharacter.h"
#include "SPlayerState.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su.SpawnBots"), true, TEXT("Enable spawning of bots via timer"), ECVF_Cheat);

ASGameModeBase::ASGameModeBase()
{
	SpawnTimerInterval = 2.0f;

	KillingBonus = 1;
}

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ASGameModeBase::SpawnBotsTimerElapsed, SpawnTimerInterval, true);
}

void ASGameModeBase::SpawnBotsTimerElapsed()
{
	if (!CVarSpawnBots.GetValueOnGameThread()) {

		UE_LOG(LogTemp, Warning, TEXT("Bots spawning disabled via cvar 'CVarSpawnBots'."));
		return;
	}

	int32 NumOfAliveBots = 0;
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It) {

		ASAICharacter* Bot = *It;

		USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(Bot->GetComponentByClass(USAttributeComponent::StaticClass()));
		if (AttributeComp && AttributeComp->IsAlive()) {
			NumOfAliveBots++;
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Find %d alive bots"), NumOfAliveBots);

	float MaxBotsCount = 10.0f;
	if (DifficultyCurve) {
		MaxBotsCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}

	if (NumOfAliveBots >= MaxBotsCount) {
		UE_LOG(LogTemp, Log, TEXT("At maximun bot capacity"));
		return;
	}

	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(GetWorld(), SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
	if (ensure(QueryInstance)) {
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnQueryCompleted);
	}

}

void ASGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success) {
		UE_LOG(LogTemp, Warning, TEXT("Spawn bots EQS Query failed"));
		return;
	}

	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
	if (Locations.IsValidIndex(0)) {

		GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);
		
		DrawDebugSphere(GetWorld(), Locations[0], 50.0f, 20, FColor::Blue, false, 60.0f);
	}
}

void ASGameModeBase::KillAll()
{
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It) {

		ASAICharacter* Bot = *It;

		USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(Bot->GetComponentByClass(USAttributeComponent::StaticClass()));
		if (AttributeComp && AttributeComp->IsAlive()) {
			AttributeComp->Kill(this);	// Use game mode contemporary
		}
	}
}

void ASGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	if (ensure(Controller)) {

		Controller->UnPossess();

		RestartPlayer(Controller);
	}
}

void ASGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
	ASCharacter* Player = Cast<ASCharacter>(VictimActor);
	if (Player) {

		FTimerHandle TimerHandle_RespawnDelay;

		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "RespawnPlayerElapsed", Player->GetController());

		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate, 2.0f, false);
	}
}

void ASGameModeBase::OnMinionKilled(AActor* VictimActor, AActor* Killer)
{
	ASAICharacter* Minion = Cast<ASAICharacter>(VictimActor);
	if (Minion) {

		ASCharacter* Player = Cast<ASCharacter>(Killer);
		if (Player) {

			ASPlayerState* PS = Cast<ASPlayerState>(Player->GetPlayerState());
			PS->ApplyCreditsChanged(KillingBonus);
		}
	}
}
