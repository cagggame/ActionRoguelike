// Fill out your copyright notice in the Description page of Project Settings.


#include "SActionEffect_Thorns.h"
#include "SActionComponent.h"
#include <SAttributeComponent.h>
#include "SGameplayFunctionLibrary.h"

USActionEffect_Thorns::USActionEffect_Thorns()
{
	ReflectionFraction = 0.1;

	Period = 0.0f;
	Duration = 0.0f;
}

void USActionEffect_Thorns::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	USAttributeComponent* AttributeComp = USAttributeComponent::GetAttribute(GetOwningActionComp()->GetOwner());
	if (ensure(AttributeComp)) {

		AttributeComp->OnHealthChanged.AddDynamic(this, &USActionEffect_Thorns::OnHealthChanged);
	}
}

void USActionEffect_Thorns::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);

	USAttributeComponent* AttributeComp = USAttributeComponent::GetAttribute(GetOwningActionComp()->GetOwner());
	if (ensure(AttributeComp)) {

		AttributeComp->OnHealthChanged.RemoveDynamic(this, &USActionEffect_Thorns::OnHealthChanged);
	}
}

void USActionEffect_Thorns::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	AActor* OwningActor = GetOwningActionComp()->GetOwner();

	if (Delta < 0.0f && OwningActor != InstigatorActor) {

		int32 Damage = FMath::RoundToInt(Delta * ReflectionFraction);

		USGameplayFunctionLibrary::ApplyDamage(OwningActor, InstigatorActor, Damage);

		UE_LOG(LogTemp, Log, TEXT("Thorns damage: %f"), Damage);
	}
	
}

