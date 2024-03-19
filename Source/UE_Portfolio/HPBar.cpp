// Fill out your copyright notice in the Description page of Project Settings.


#include "HPBar.h"
#include "Components/ProgressBar.h"

void UHPBar::UpdateHPBar(float percentage)
{
	if (HPBar)
	{
		HPBar->SetPercent(percentage);
		SetHpBarVisible(true);

		if (HPBar->GetPercent() <= 0)
		{
			HPBar->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			if (!bAlwaysVisible)
			{
				FTimerDelegate VisibleDelegate = FTimerDelegate::CreateUObject(this, &UHPBar::SetHpBarVisible, false);

				if (GetWorld())
				{
					GetWorld()->GetTimerManager().SetTimer(VisibleUpdateTimerHandle, VisibleDelegate, Duriation, false);
				}
			}
			else
			{
				if(GetWorld() && GetWorld()->GetTimerManager().IsTimerActive(VisibleUpdateTimerHandle))
				{
					GetWorld()->GetTimerManager().ClearTimer(VisibleUpdateTimerHandle);
				}
			}
		}
	}
}

void UHPBar::SetHpBarVisible(bool bIsAct)
{
	HPBar->SetVisibility(bIsAct ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UHPBar::AlwaysVisible(bool bIsAct)
{
	bAlwaysVisible = bIsAct;

	if (!bAlwaysVisible)
	{
		FTimerDelegate VisibleDelegate = FTimerDelegate::CreateUObject(this, &UHPBar::SetHpBarVisible, bAlwaysVisible);

		if (GetWorld())
		{
			GetWorld()->GetTimerManager().SetTimer(VisibleUpdateTimerHandle, VisibleDelegate, Duriation, false);
		}
	}
	else
	{
		SetHpBarVisible(bAlwaysVisible);
	}
}
