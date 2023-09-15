// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SAICharacter.generated.h"

class UPawnSensingComponent;
class USAttributeComponent;
class USWorldUserWidget;
class USActionComponent;

UCLASS()
class ACTIONROGUELIKE_API ASAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASAICharacter();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Component")
	UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	USAttributeComponent* AttributeComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USActionComponent* ActionComp;

	UPROPERTY(VisibleAnywhere, Category = "Effect")
	FName TimeToHitParmName;

	USWorldUserWidget* ActivateHealthBar;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<USWorldUserWidget> SpottedWidgetClass;

	USWorldUserWidget* SpottedWidgetInstance;

	/*USWorldUserWidget* SpottedWidgetInstance;*/

	virtual void PostInitializeComponents() override;

	void SetTargetActor(AActor* NewTarget);

	AActor* GetTargetActor() const;

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastPawnSeen();

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

};
