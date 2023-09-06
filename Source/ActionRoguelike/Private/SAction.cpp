// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction.h"
#include "SActionComponent.h"
#include "../ActionRoguelike.h"
#include "Net/UnrealNetwork.h"

void USAction::Initialize(USActionComponent* NewActionComp)
{
	ActionComponent = NewActionComp;
}

void USAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));
	//LogOnScreen(this, FString::Printf(TEXT("Started: %s"), *ActionName.ToString()), FColor::Green);

	USActionComponent* ActionComp = GetOwningActionComp();
	ActionComp->ActivateGameplayTags.AppendTags(GrantTags);
	
	RepDatas.bIsRunning = true;
	RepDatas.Instigator = Instigator;
}

void USAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopped: %s"), *GetNameSafe(this));
	//LogOnScreen(this, FString::Printf(TEXT("Stopped: %s"), *ActionName.ToString()), FColor::White);

	//ensureAlways(bIsRunning);

	USActionComponent* ActionComp = GetOwningActionComp();
	ActionComp->ActivateGameplayTags.RemoveTags(BlockTags);

	RepDatas.bIsRunning = false;
	RepDatas.Instigator = Instigator;
}

USActionComponent* USAction::GetOwningActionComp() const
{
	return ActionComponent;
}

UWorld* USAction::GetWorld() const
{
	AActor* Actor = Cast<AActor>(GetOuter());
	if (Actor) {

		return Actor->GetWorld();
	}

	return nullptr;
}

bool USAction::IsRunning() const
{
	return RepDatas.bIsRunning;
}

bool USAction::CanStart_Implementation(AActor* InstigatorActor)
{
	if (IsRunning()) {

		return false;
	}

	USActionComponent* ActionComp = GetOwningActionComp();
	if (ActionComp->ActivateGameplayTags.HasAny(BlockTags)) {

		return false;
	}

	return true;
}

void USAction::OnRep_RepDatas()
{
	if (RepDatas.bIsRunning) {
		StartAction(RepDatas.Instigator);
	}
	else {
		StopAction(RepDatas.Instigator);
	}
}

void USAction::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USAction, RepDatas);
	DOREPLIFETIME(USAction, ActionComponent);
}