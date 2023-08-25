// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction_ProjectileAttack.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

USAction_ProjectileAttack::USAction_ProjectileAttack()
{
	HandSocketName = "Muzzle_01";

	AttackAnimDelay = 0.2f;
}

void USAction_ProjectileAttack::StartAction_Implementation(AActor* InstigatorActor)
{
	Super::StartAction_Implementation(InstigatorActor);

	ACharacter* Character = Cast<ACharacter>(InstigatorActor);
	if (Character) {

		Character->PlayAnimMontage(AttackAnim);

		// UGameplayStatics::SpawnEmitterAttached(CastingEffect, Character->GetMesh(), HandSocketName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);

		FTimerHandle  TimerHandle_AttackDelay;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "AttackDelay_Elapsed", Character);

		GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay, Delegate, AttackAnimDelay, false);
	}
}

void USAction_ProjectileAttack::AttackDelay_Elapsed(ACharacter* InstigatorCharacter)
{
	if (ensure(ProjectileClass)) {
		FVector SpawnLocation = InstigatorCharacter->GetMesh()->GetSocketLocation(HandSocketName);

		// Find hit location through line trace, then recalculate SpawnRotation by subtraction of SpawnLocation and hit location
		FHitResult Hit;

		FVector CameraLocation = InstigatorCharacter->GetPawnViewLocation();
		FVector TraceEnd = CameraLocation + (InstigatorCharacter->GetControlRotation().Vector() * 5000.0f);

		FCollisionObjectQueryParams ObjectQueryParams;
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);

		FCollisionShape Shape;
		Shape.SetSphere(20.0f);

		FCollisionQueryParams Params;
		Params.AddIgnoredActor(InstigatorCharacter);

		bool bIsHit = GetWorld()->SweepSingleByObjectType(Hit, CameraLocation, TraceEnd, FQuat::Identity, ObjectQueryParams, Shape, Params);

		FVector HitLocation = TraceEnd;
		if (bIsHit) {
			HitLocation = Hit.ImpactPoint;
		}

		FRotator SpawnRotation = FRotationMatrix::MakeFromX(HitLocation - SpawnLocation).Rotator();

		FActorSpawnParameters SpawnParameters;
		// Make sure the projectile will not collide player's hand
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParameters.Instigator = InstigatorCharacter;

		UGameplayStatics::SpawnEmitterAttached(CastingEffect, InstigatorCharacter->GetMesh(), HandSocketName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);

		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParameters);
	}

	StopAction(InstigatorCharacter);
}
