// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"
#include "SGameplayInterface.h"
#include "DrawDebugHelpers.h"
#include "SWorldUserWidget.h"

static TAutoConsoleVariable<bool> CVarDebugDrawInteraction(TEXT("su.InteractionDebugDraw"), 1.0f, TEXT("Enable debug line for interaction component"), ECVF_Cheat);

// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	TraceDistance = 500.0f;
	TraceRadius = 30.0f;
	CollisionChannel = ECC_WorldDynamic;
}


// Called when the game starts
void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FindBestInteractable();
}

void USInteractionComponent::FindBestInteractable()
{
	bool bDebugDraw = CVarDebugDrawInteraction.GetValueOnGameThread();

	TArray<FHitResult> Hits;

	AActor* MyOwner = GetOwner();

	FVector EyeLocation;
	FRotator EyeRotation;
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector TraceEnd = EyeLocation + (EyeRotation.Vector() * TraceDistance);

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);

	FCollisionShape Shape;
	Shape.SetSphere(TraceRadius);

	// Clear the focus actor
	FocusActor = nullptr;

	bool bIsHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, TraceEnd, FQuat::Identity, ObjectQueryParams, Shape);

	FColor DebugColor = bIsHit ? FColor::Green : FColor::Red;
	//if (bDebugDraw) {
	//	DrawDebugLine(GetWorld(), EyeLocation, TraceEnd, DebugColor, false, 2.0f, 0, 2.0f);
	//}

	for (FHitResult Hit : Hits) {
		AActor* HitActor = Hit.GetActor();
		if (HitActor) {

			if (HitActor->Implements<USGameplayInterface>()) {

				if (bDebugDraw) {
					DrawDebugSphere(GetWorld(), Hit.ImpactPoint, TraceRadius, 16, DebugColor, false, 2.0f);
				}

				FocusActor = HitActor;
				break;
			}
		}
	}

	if (FocusActor) {

		if (DefaultWidgetInstance == nullptr && ensure(DefaultWidgetClass)) {

			DefaultWidgetInstance = CreateWidget<USWorldUserWidget>(GetWorld(), DefaultWidgetClass);
		}

		if (DefaultWidgetInstance) {

			DefaultWidgetInstance->AttachedActor = FocusActor;
			if (!DefaultWidgetInstance->IsInViewport()) {

				DefaultWidgetInstance->AddToViewport();
			}	
		}
	}
	else {

		if (DefaultWidgetInstance) {

			DefaultWidgetInstance->RemoveFromParent();
		}
	}
}

void USInteractionComponent::PrimaryInteract()
{
	if (FocusActor == nullptr) {

		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "No focus actor to interact");
		return;
	}

	APawn* MyPawn = Cast<APawn>(GetOwner());
	ISGameplayInterface::Execute_Interact(FocusActor, MyPawn);
}


