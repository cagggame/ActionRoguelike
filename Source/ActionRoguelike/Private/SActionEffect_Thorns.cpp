// Fill out your copyright notice in the Description page of Project Settings.


#include "SActionEffect_Thorns.h"
#include "SActionComponent.h"
#include <SAttributeComponent.h>
#include "SGameplayFunctionLibrary.h"

USActionEffect_Thorns::USActionEffect_Thorns()
{
	ReflectedDamageRate = 0.1;

	bAutoStart = true;
}

void USActionEffect_Thorns::StartAction_Implementation(AActor* Instigator)
{
	AActor* MyOwner = GetOwningActionComp()->GetOwner();
	if (ensure(MyOwner)) {

		USAttributeComponent* AttributeComp = USAttributeComponent::GetAttribute(MyOwner);
		if (ensure(AttributeComp)) {

			AttributeComp->OnHealthChanged.AddDynamic(this, &USActionEffect_Thorns::OnHealthChanged);

			StopAction_Implementation(Instigator);
		}
	}
}

void USActionEffect_Thorns::StopAction_Implementation(AActor* Instigator)
{
	AActor* MyOwner = GetOwningActionComp()->GetOwner();
	if (ensure(MyOwner)) {

		USAttributeComponent* AttributeComp = USAttributeComponent::GetAttribute(MyOwner);
		if (ensure(AttributeComp)) {

			AttributeComp->OnHealthChanged.RemoveDynamic(this, &USActionEffect_Thorns::OnHealthChanged);
		}
	}
}

void USActionEffect_Thorns::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.0f) {

		float Damage = FMath::RoundToInt(Delta * ReflectedDamageRate);

		USGameplayFunctionLibrary::ApplyDamage(GetOwningActionComp()->GetOwner(), InstigatorActor, Damage);

		UE_LOG(LogTemp, Log, TEXT("Thorns damage: %f"), Damage);
	}
	
}

