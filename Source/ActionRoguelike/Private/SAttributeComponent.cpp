// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"
#include "SGameModeBase.h"

static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("su.DamageMultiplier"), 1.0f, TEXT("Gloable damage modifier for attribute component"), ECVF_Cheat);

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	Health = 100.0f;
	HealthMax = 100.0f;

	Rage = 50.0f;
	RageMax = 100.0f;
}

USAttributeComponent* USAttributeComponent::GetAttribute(AActor* FromActor)
{
	if (FromActor) {

		return Cast<USAttributeComponent>(FromActor->GetComponentByClass(USAttributeComponent::StaticClass()));
	}

	return nullptr;
}

bool USAttributeComponent::IsActorAlive(AActor* Actor)
{
	USAttributeComponent* AttributeComp = GetAttribute(Actor);
	if (AttributeComp) {

		return AttributeComp->IsAlive();
	}

	return false;
}

bool USAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -GetHealthMax());
}

bool USAttributeComponent::IsAlive() const
{
	return Health > 0;
}

bool USAttributeComponent::IsFullHealth() const
{
	return Health == HealthMax;
}

bool USAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	if (!GetOwner()->CanBeDamaged() && Delta < 0.0f) {
		return false;
	}

	if (Delta < 0.0f) {

		float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();
		Delta *= DamageMultiplier;
	}

	float OldHealth = Health;
	Health = FMath::Clamp(Health + Delta, 0.0f, HealthMax);

	float ActualDelta = Health - OldHealth;
	OnHealthChanged.Broadcast(InstigatorActor, this, Health, ActualDelta);

	if (ActualDelta < 0.0f && Health == 0.0f) {

		ASGameModeBase* GM = Cast<ASGameModeBase>(GetWorld()->GetAuthGameMode());
		if (GM) {

			GM->OnActorKilled(GetOwner(), InstigatorActor);
			GM->OnMinionKilled(GetOwner(), InstigatorActor);
		}
	}

	return ActualDelta != 0;
}

float USAttributeComponent::GetHealth() const
{
	return Health;
}

float USAttributeComponent::GetHealthMax() const
{
	return HealthMax;
}

bool USAttributeComponent::AddRage(AActor* InstigatorActor, float Delta)
{
	if (Delta > 0) {

		Rage = FMath::Clamp(Rage + Delta, 0.0f, RageMax);
		OnRageChanged.Broadcast(InstigatorActor, this, Rage, Delta);

		return true;
	}

	return false;
}

bool USAttributeComponent::RemoveRage(AActor* InstigatorActor, float Delta)
{
	if (!ensure(Delta > 0)) {
		return false;
	}

	if (Rage < Delta) {
		return false;
	}

	Rage = FMath::Clamp(Rage - Delta, 0.0f, RageMax);
	OnRageChanged.Broadcast(InstigatorActor, this, Rage, -Delta);

	return true;
}

float USAttributeComponent::GetRage() const
{
	return Rage;
}

float USAttributeComponent::GetRageMax() const
{
	return RageMax;
}
