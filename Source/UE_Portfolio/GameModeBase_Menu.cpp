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

			if (CurrentLevelName.StartsWith(MenuName))
			{
				CreatedWidget->SetGroupVisibility(EVisibleType::MenuGroup);
			}
			else
			{
				CreatedWidget->SetGroupVisibility(EVisibleType::None);
			}

		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Widget creation failed."));
		}
	}
	else
	{ 
		UE_LOG(LogTemp, Error, TEXT("MenuWidget class is not set."));
	}
}
