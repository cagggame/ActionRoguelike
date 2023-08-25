// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction.h"
#include "SActionComponent.h"

void USAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));

	USActionComponent* ActionComp = GetOwningActionComp();
	ActionComp->ActivateGameplayTags.AppendTags(GrantTags);

	bIsRunning = true;
}

void USAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopped: %s"), *GetNameSafe(this));

	ensureAlways(bIsRunning);

	USActionComponent* ActionComp = GetOwningActionComp();
	ActionComp->ActivateGameplayTags.RemoveTags(BlockTags);

	bIsRunning = false;
}

USActionComponent* USAction::GetOwningActionComp() const
{
	return Cast<USActionComponent>(GetOuter());
}

UWorld* USAction::GetWorld() const
{
	USActionComponent* ActionComp = Cast<USActionComponent>(GetOuter());
	if (ActionComp) {

		return ActionComp->GetWorld();
	}

	return nullptr;
}

bool USAction::IsRunning() const
{
	return bIsRunning;
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
