// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "SAction.generated.h"

class UWorld;

USTRUCT()
struct FActionRepDatas {

	GENERATED_BODY()

public:
	UPROPERTY()
	bool bIsRunning;

	UPROPERTY()
	AActor* Instigator;
};

/**
 * 
 */
UCLASS(Blueprintable)
class ACTIONROGUELIKE_API USAction : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSoftObjectPtr<UTexture2D> Icon;

	UPROPERTY(Replicated)
	USActionComponent* ActionComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer GrantTags;

	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer BlockTags;

	UFUNCTION(BlueprintCallable, Category = "Actions")
	USActionComponent* GetOwningActionComp() const;
	
public:
	UPROPERTY(Replicated)
	float TimeStarted;

	void Initialize(USActionComponent* NewActionComp);

	UPROPERTY(EditDefaultsOnly, Category = "Actions")
	bool bAutoStart;

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	void StopAction(AActor* Instigator);

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName;

	UPROPERTY(ReplicatedUsing = "OnRep_RepDatas")
	FActionRepDatas RepDatas;

	UWorld* GetWorld() const override;

	UFUNCTION(BlueprintNativeEvent, Category = "Actions")
	bool CanStart(AActor* InstigatorActor);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool IsRunning() const;

	UFUNCTION()
	void OnRep_RepDatas();

	bool IsSupportedForNetworking() const override 
	{
		return true;
	}
};
