#include "GameUI.h"
#include "Components/ProgressBar.h"

void UGameUI::PlayEnterdAnimation()
{
	if (Begin)
	{
		PlayAnimation(Begin);
	}
}

void UGameUI::UpdateHPBar(float percentage)
{
	if (HPBar)
	{
		HPBar->SetPercent(percentage);
	}
}

void UGameUI::UpdateStaminaBar(float percentage)
{
	if (StaminaBar)
	{
		StaminaBar->SetPercent(percentage);
	}
}