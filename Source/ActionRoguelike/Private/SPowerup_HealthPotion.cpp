// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerup_HealthPotion.h"
#include "SAttributeComponent.h"
#include "SPlayerState.h"

ASPowerup_HealthPotion::ASPowerup_HealthPotion()
{
	Heal = 20.0f;

	CreditsCost = 50;
}

void ASPowerup_HealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	if (InstigatorPawn) {

		ASPlayerState* PS = Cast<ASPlayerState>(InstigatorPawn->GetPlayerState());
		if (PS) {
			
			int32 Credits = PS->GetCredits();
			if (Credits < 1) {
				return;
			}

			USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(InstigatorPawn->GetComponentByClass(USAttributeComponent::StaticClass()));
			if (AttributeComp && !AttributeComp->IsFullHealth()) {
				
				if (PS->RemoveCredits(CreditsCost) && AttributeComp->ApplyHealthChange(this, Heal)) {
					Inactivate();
				}
			}
		}
	}
}
