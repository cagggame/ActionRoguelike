// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class USInteractionComponent;

UCLASS()
class ACTIONROGUELIKE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCharacter();

protected:
	FTimerHandle TimerHandle_PrimaryAttack;
	FTimerHandle TimerHandle_DashProjectile;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere)
	USInteractionComponent* InteractionComp;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	TSubclassOf<AActor> MagicProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	TSubclassOf<AActor> DashProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	UAnimMontage* AttackAnim;

	UPROPERTY(VisibleAnywhere, Category = "Attack")
	FName MuzzleName;

protected:
	void MoveForward(float Value);

	void MoveRight(float Value);

	void SpwanProjectile(TSubclassOf<AActor> ProjectileClass);

	void PrimaryAttack();

	void PrimaryAttack_Elapsed();

	void DashProjectile();

	void DashProjectile_Elapsed();

	void PrimaryInteract();

public:	

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
