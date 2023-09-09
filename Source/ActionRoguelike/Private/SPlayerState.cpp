// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "SSaveGame.h"

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

void ASPlayerState::OnRep_CreditsChanged(int32 OldCredits)
{
	OnCreditsChanged.Broadcast(this, Credits, Credits - OldCredits);
}

void ASPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const 
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASPlayerState, Credits);
}


void ASPlayerState::SavePlayerState_Implementation(USSaveGame* SaveObject)
{
	if (SaveObject) {

		SaveObject->Credits = Credits;
	}
}

void ASPlayerState::LoadPlayerState_Implementation(USSaveGame* SaveObject)
{
	if (SaveObject) {

		Credits = SaveObject->Credits;
	}
}
