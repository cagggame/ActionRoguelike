// Fill out your copyright notice in the Description page of Project Settings.


#include "SWorldUserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SizeBox.h"
#include "Blueprint/WidgetLayoutLibrary.h"

void USWorldUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!IsValid(AttachedActor)) {

		RemoveFromParent();

		return;
	}
	 
	FVector2D ScreenPosition;
	bool bIsOnSreen = UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), AttachedActor->GetActorLocation() + WorldOffset, ScreenPosition);

	if (bIsOnSreen) 
	{
		float Scale = UWidgetLayoutLibrary::GetViewportScale(this);

		ScreenPosition /= Scale;

		if (ParentSizeBox)
		{
			ParentSizeBox->SetRenderTranslation(ScreenPosition);
		}
	}

	if (ParentSizeBox)
	{
		ParentSizeBox->SetVisibility(bIsOnSreen ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	}
}
