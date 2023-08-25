// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerup_Credits.h"
#include "SPlayerState.h"

ASPowerup_Credits::ASPowerup_Credits()
{
	CreditsBonus = 80;
}

void ASPowerup_Credits::Interact_Implementation(APawn* InstigatorPawn)
{
	if (InstigatorPawn) {

		ASPlayerState* PS = Cast<ASPlayerState>(InstigatorPawn->GetPlayerState());
		if (PS) {

			PS->AddCredits(CreditsBonus);

			Inactivate();
		}
	}
}
