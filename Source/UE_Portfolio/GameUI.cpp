#include "GameUI.h"
#include "Components/ProgressBar.h"

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