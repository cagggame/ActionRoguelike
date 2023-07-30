// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPowerupActor.h"
#include "SPowerup_HealthPotion.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASPowerup_HealthPotion : public ASPowerupActor
{
	GENERATED_BODY()

public:
	ASPowerup_HealthPotion();

protected:
	UPROPERTY(EditAnywhere)
	float Heal;

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
};
