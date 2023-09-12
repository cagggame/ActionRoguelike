// Fill out your copyright notice in the Description page of Project Settings.


#include "SAnimInstance.h"
#include "SActionComponent.h"
#include "GameplayTagContainer.h"

void USAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	AActor* OwningActor = GetOwningActor();
	if (OwningActor) 
	{
		ActionComp = Cast<USActionComponent>(OwningActor->GetComponentByClass(USActionComponent::StaticClass()));
	}
}

void USAnimInstance::NativeUpdateAnimation(float DeltaTimeX)
{
	Super::NativeUpdateAnimation(DeltaTimeX);

	static FGameplayTag StunnedTag = FGameplayTag::RequestGameplayTag("Status.Stunned");
	if (ActionComp) 
	{
		bIsStunned = ActionComp->ActivateGameplayTags.HasTag(StunnedTag);
	}
}
