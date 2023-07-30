// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerupActor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"

// Sets default values
ASPowerupActor::ASPowerupActor()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	RootComponent = SphereComp;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetupAttachment(RootComponent);

	RespawnTime = 10.0f;
}

void ASPowerupActor::Interact_Implementation(APawn* InstigatorPawn)
{
	// implementation in derived class
}

void ASPowerupActor::Inactivate()
{
	SetVisibilityAndCollision(false);

	Respawn();
}

void ASPowerupActor::Respawn()
{
	FTimerHandle TimerHandle_ShowPowerup;
	GetWorldTimerManager().SetTimer(TimerHandle_ShowPowerup, this, &ASPowerupActor::ShowPowerup, RespawnTime);
}

void ASPowerupActor::ShowPowerup()
{
	SetVisibilityAndCollision(true);
}

void ASPowerupActor::SetVisibilityAndCollision(bool bIsActivate)
{
	SetActorHiddenInGame(!bIsActivate);

	SetActorEnableCollision(bIsActivate);
}

