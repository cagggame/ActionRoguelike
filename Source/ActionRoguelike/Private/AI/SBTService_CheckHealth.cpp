// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_CheckHealth.h"
#include "AIController.h"
#include "SAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

void USBTService_CheckHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	if (ensure(BBComp)) {

		AAIController* AIC = OwnerComp.GetAIOwner();
		if (ensure(AIC)) {

			APawn* MyPawn = AIC->GetPawn();
			if (ensure(MyPawn)) {

				USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(MyPawn->GetComponentByClass(USAttributeComponent::StaticClass()));
				if (AttributeComp) {

					bool bIsAiLowHealth = AttributeComp->IsLowHealth();
					BBComp->SetValueAsBool("IsLowHealth", bIsAiLowHealth);
				}
			}
		}
	}
}
