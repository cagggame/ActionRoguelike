// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "SAction.generated.h"

class UWorld;

/**
 * 
 */
UCLASS(Blueprintable)
class ACTIONROGUELIKE_API USAction : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer GrantTags;

	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer BlockTags;

	UFUNCTION(BlueprintCallable, Category = "Actions")
	USActionComponent* GetOwningActionComp() const;
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "Actions")
	bool bAutoStart;

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	void StopAction(AActor* Instigator);

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName;

	bool bIsRunning;

	UWorld* GetWorld() const override;

	UFUNCTION(BlueprintNativeEvent, Category = "Actions")
	bool CanStart(AActor* InstigatorActor);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool IsRunning() const;
};
