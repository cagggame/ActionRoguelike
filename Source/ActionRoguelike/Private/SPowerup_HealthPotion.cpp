// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerup_HealthPotion.h"
#include <SAttributeComponent.h>

ASPowerup_HealthPotion::ASPowerup_HealthPotion()
{
	Heal = 20.0f;
}

void ASPowerup_HealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	if (InstigatorPawn) {
		USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(InstigatorPawn->GetComponentByClass(USAttributeComponent::StaticClass()));
		if (AttributeComp && !AttributeComp->IsFullHealth()) {
			AttributeComp->ApplyHealthChange(this, 20.0f);

			Inactivate();
		}
	}
}
