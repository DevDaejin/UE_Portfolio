// Fill out your copyright notice in the Description page of Project Settings.


#include "HPBar.h"
#include "Components/ProgressBar.h"

void UHPBar::UpdateHPBar(float percentage)
{
	if (HPBar)
	{
		HPBar->SetPercent(percentage);
	}
}
