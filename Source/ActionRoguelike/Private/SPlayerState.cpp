// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"

ASPlayerState::ASPlayerState()
{
	Credits = 0;
}

void ASPlayerState::AddCredits(float Deltas)
{
	if (!ensure(Deltas > 0)) {
		return;
	}

	Credits += Deltas;

	OnCreditsChanged.Broadcast(this, Credits, Deltas);
}

bool ASPlayerState::RemoveCredits(float Deltas)
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

int32 ASPlayerState::GetCredits()
{
	return Credits;
}
