// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectileBase.h"
#include "Particles/ParticleSystem.h"
#include "SDashProjectile.generated.h"

class UParticleSystem;

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASDashProjectile : public ASProjectileBase
{
	GENERATED_BODY()
	
public:
	ASDashProjectile();

protected:
	FTimerHandle TimerHandle_Explode;

	virtual void BeginPlay() override;

	virtual void Explode_Implementation() override;

	void Teleport();
};
