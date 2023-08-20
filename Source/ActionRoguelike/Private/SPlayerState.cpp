// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"

ASPlayerState::ASPlayerState()
{
	Credits = 1;
}

void ASPlayerState::ApplyCreditsChanged(float Delta)
{
	Credits += Delta;

	if (Credits <= 0) {
		Credits = 0;
	}
}

int32 ASPlayerState::GetCredits()
{
	return Credits;
}
