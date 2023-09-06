// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"
#include "Net/UnrealNetwork.h"

ASPlayerState::ASPlayerState()
{
	Credits = 80;

	SetReplicates(true);
}

void ASPlayerState::AddCredits(int32 Deltas)
{
	if (!ensure(Deltas > 0)) {
		return;
	}

	Credits += Deltas;

	OnCreditsChanged.Broadcast(this, Credits, Deltas);
}

bool ASPlayerState::RemoveCredits(int32 Deltas)
{
	if (!ensure(Deltas >= 0)) {
		return false;
	}

	if (Credits < Deltas) {
		return false;
	}

	Credits -= Deltas;

	OnCreditsChanged.Broadcast(this, Credits, -Deltas);

	return true;
}

int32 ASPlayerState::GetCredits() const
{
	return Credits;
}

void ASPlayerState::MulticastCredisChanged_Implementation(int32 NewCredits, int32 Deltas)
{
	OnCreditsChanged.Broadcast(this, Credits, Deltas);
}

void ASPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const 
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASPlayerState, Credits);
}