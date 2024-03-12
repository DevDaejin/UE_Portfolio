#include "GameUI.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"

void UGameUI::PlayEnterdAnimation()
{
	PlayAnimation(Begin);
}

void UGameUI::UpdateHPBar(float percentage)
{
	HPBar->SetPercent(percentage);
}

void UGameUI::UpdateStaminaBar(float percentage)
{
	StaminaBar->SetPercent(percentage);
}

void UGameUI::SetLockOnWidgetPosition(FVector2D& Position)
{
	if (Target)
	{
		Target->SetVisibility(ESlateVisibility::Collapsed);
		UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Target->Slot);

		if (CanvasSlot)
		{
			CanvasSlot->SetPosition(Position);
		}
	}
}

void UGameUI::DeactiveTarget()
{
	if (Target)
	{
		Target->SetVisibility(ESlateVisibility::Hidden);
	}
}
