// Fill out your copyright notice in the Description page of Project Settings.


#include "SDashProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ASDashProjectile::ASDashProjectile()
{
}

void ASDashProjectile::BeginPlay()
{
	/*AActor* MyOwner = Cast<AActor>(GetInstigator());
	if (MyOwner) {
		SphereComp->IgnoreActorWhenMoving(MyOwner, true);
	}*/
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_Explode, this, &ASDashProjectile::Explode, 0.2f);
}

void ASDashProjectile::Explode_Implementation()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_Explode);

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactVFX, GetActorLocation(), GetActorRotation(), true);

	ParticleComp->DeactivateSystem();
	MovementComp->StopMovementImmediately();

	FTimerHandle TimerHandle_Teleport;
	GetWorldTimerManager().SetTimer(TimerHandle_Teleport, this, &ASDashProjectile::Teleport, 0.2f);
}

void ASDashProjectile::Teleport()
{
	APawn* MyOwner = GetInstigator();
	if (ensure(MyOwner)) {
		// MyOwner->SetActorLocation(GetActorLocation());
		MyOwner->TeleportTo(GetActorLocation(), MyOwner->GetActorRotation());

		Destroy();
	}
}
