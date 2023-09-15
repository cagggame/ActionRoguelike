// Fill out your copyright notice in the Description page of Project Settings.


#include "SActionComponent.h"
#include "SAction.h"
#include "../ActionRoguelike.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"

DECLARE_CYCLE_STAT(TEXT("StartActionByName"), SATA_StartActionByName, STATGROUP_OWNSTAT);

USActionComponent::USActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
}

void USActionComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner()->HasAuthority()) {
		for (TSubclassOf<USAction> ActionClass : DefaultActions) {

			if (ActionClass) {
				AddAction(GetOwner(), ActionClass);
			}
		}
	}
	
}

void USActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/*FString DebugMsg = GetNameSafe(GetOwner()) + " : " + ActivateGameplayTags.ToStringSimple();
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, DebugMsg);*/

	for (USAction* Action : Actions) {

		FColor TextColor = Action->IsRunning() ? FColor::Blue : FColor::White;

		FString ActionMsg = FString::Printf(TEXT("[%s] Action: %s"), *GetNameSafe(GetOwner()), *GetNameSafe(Action));

		LogOnScreen(this, ActionMsg, TextColor, 0.0f);
	}
}

void USActionComponent::AddAction(AActor* Instigator, TSubclassOf<USAction> ActionClass)
{
	if (!ensure(ActionClass)) {
		return;
	}

	if (!GetOwner()->HasAuthority()) {

		UE_LOG(LogTemp, Warning, TEXT("Client attempting to AddAction. [Class: %s]"), *GetNameSafe(ActionClass));
		return;
	}

	USAction* NewAction = NewObject<USAction>(GetOwner(), ActionClass);
	if (ensure(NewAction)) {

		NewAction->Initialize(this);

		Actions.Add(NewAction);

		if (NewAction->bAutoStart && ensure(NewAction->CanStart(Instigator))) {

			NewAction->StartAction(Instigator);
		}
	}
}

void USActionComponent::RemoveAction(USAction* ActionToRemove)
{
	if (!ensure(ActionToRemove && !ActionToRemove->IsRunning())) {
		return;
	}

	Actions.Remove(ActionToRemove);
}

bool USActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	SCOPE_CYCLE_COUNTER(SATA_StartActionByName);

	for (USAction* Action : Actions) {

		if (Action && Action->ActionName == ActionName) {

			if (!Action->CanStart(Instigator)) {

				FString FailMsg = FString::Printf(TEXT("Failed to run: %s"), *ActionName.ToString());
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FailMsg);
				continue;
			}

			if (!GetOwner()->HasAuthority()) {

				ServerStartAction(Instigator, ActionName);
			}

			TRACE_BOOKMARK(TEXT("StartAction::%s"), *GetNameSafe(Action));

			Action->StartAction(Instigator);
			return true;
		}
	}

	return false;
}

bool USActionComponent::StopActionByName(AActor* Instigator, FName ActionName)
{
	for (USAction* Action : Actions) {

		if (Action && Action->ActionName == ActionName) {

			if (Action->IsRunning()) {

				if (!GetOwner()->HasAuthority()) {

					ServerStopAction(Instigator, ActionName);
				}

				Action->StopAction(Instigator);
				return true;
			}
		}
	}

	return false;
}

void USActionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	TArray<USAction*> ActionCopy = Actions;
	for (USAction* Action : ActionCopy)
	{
		if (Action && Action->IsRunning())
		{
			Action->StopAction(GetOwner());
		}
	}

	Super::EndPlay(EndPlayReason);
}


void USActionComponent::ServerStartAction_Implementation(AActor* Instigator, FName ActionName)
{
	StartActionByName(Instigator, ActionName);
}

void USActionComponent::ServerStopAction_Implementation(AActor* Instigator, FName ActionName)
{
	StopActionByName(Instigator, ActionName);
}

bool USActionComponent::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WorteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	for (USAction* Action : Actions) {

		if (Action) {

			WorteSomething = Channel->ReplicateSubobject(Action, *Bunch, *RepFlags);
		}
	}

	return WorteSomething;
}

void USActionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USActionComponent, Actions);
}