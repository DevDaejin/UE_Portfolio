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
		if (Target->GetVisibility() == ESlateVisibility::Hidden)
		{
			Target->SetVisibility(ESlateVisibility::Visible);
		}

		UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Target->Slot);
		if (CanvasSlot)
		{
			if (CanvasSlot->GetAnchors() != (FAnchors(0.5f, 0.5f)))
			{
				CanvasSlot->SetAnchors(FAnchors(0.5f, 0.5f));
			}

			if (CanvasSlot->GetAlignment() != (FVector2D(0.5f, 0.5f)))
			{
				CanvasSlot->SetAlignment(FVector2D(0.5f, 0.5f));
			}
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
