// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeBase_Menu.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.h"

void AGameModeBase_Menu::BeginPlay()
{
	Super::BeginPlay();

	if (MenuWidget)
	{
		UMainMenu* CreatedWidget = CreateWidget<UMainMenu>(GetWorld(), MenuWidget);
		if (CreatedWidget)
		{
			FString CurrentLevelName = GetWorld()->GetName();

			CreatedWidget->AddToViewport();

			if (CurrentLevelName == MenuName)
			{
				CreatedWidget->SetGroupVisibility(EVisibleType::MenuGroup);
			}
			if (CurrentLevelName == MainName)
			{
				CreatedWidget->SetGroupVisibility(EVisibleType::None);
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Widget is null"));
		}
	}
	else
	{ 
		UE_LOG(LogTemp, Error, TEXT("Menu widget is null"));
	}
}
