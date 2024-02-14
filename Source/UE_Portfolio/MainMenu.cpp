// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerController = GetWorld()->GetFirstPlayerController();

	if (ValidatePlayerController())
	{
		PlayerController->bShowMouseCursor = true;
	}

	if (ValidateButton(MenuNewButton, TEXT("MenuQuitButton")))
	{
		MenuNewButton->OnClicked.AddDynamic(this, &UMainMenu::OnMenuNewButtonClicked);
	}

	if (ValidateButton(MenuStartButton, TEXT("MenuStartButton")))
	{//추 후 이전 게임 Load로
		MenuStartButton->OnClicked.AddDynamic(this, &UMainMenu::OnMenuStartButtonClicked);
	}

	if (ValidateButton(MenuOptionButton, TEXT("MenuOptionButton")))
	{
		MenuOptionButton->OnClicked.AddDynamic(this, &UMainMenu::OnMenuOptionButtonClicked);
	}

	if (ValidateButton(MenuQuitButton, TEXT("MenuQuitButton")))
	{
		MenuQuitButton->OnClicked.AddDynamic(this, &UMainMenu::OnMenuQuitButtonClicked);
	}

	if (ValidateButton(OptionQuitButton, TEXT("OptionQuitButton")))
	{
		OptionQuitButton->OnClicked.AddDynamic(this, &UMainMenu::OnOptionQuitButtonClicked);
	}
}

bool UMainMenu::ValidatePlayerController()
{
	if (PlayerController)
	{
		return true;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Player controller is null"));
		return false;
	}
}

bool UMainMenu::ValidateButton(UButton* Button, const FString& ButtonName)
{
	if (Button)
	{
		return true;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s is null"), *ButtonName);
		return false;
	}
}

void UMainMenu::OnMenuNewButtonClicked()
{
	if (ValidatePlayerController())
	{
		UGameplayStatics::OpenLevel(this, NewGameLevelName);
	}
}

void UMainMenu::OnMenuStartButtonClicked()
{
	if (ValidatePlayerController())
	{
		UGameplayStatics::OpenLevel(this, NewGameLevelName);
	}
}

void UMainMenu::OnMenuOptionButtonClicked()
{
	SetGroupVisibility(EVisibleType::MainGroupWithOption);
}

void UMainMenu::OnMenuQuitButtonClicked()
{
	if (ValidatePlayerController())
	{
		PlayerController->ConsoleCommand(Quit);
	}
}

void UMainMenu::OnOptionConfirmButtonClicked()
{
	SetGroupVisibility(EVisibleType::MenuGroup);
}

void UMainMenu::OnOptionQuitButtonClicked()
{
	SetGroupVisibility(EVisibleType::MenuGroup);
}

void UMainMenu::SetGroupVisibility(EVisibleType type)
{
	switch (type)
	{
	case EVisibleType::MenuGroup:
		MenuGroup->SetVisibility(ESlateVisibility::Visible);
		OptionGroup->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case EVisibleType::Option:
		MenuGroup->SetVisibility(ESlateVisibility::Collapsed);
		OptionGroup->SetVisibility(ESlateVisibility::Visible);
		break;
	case EVisibleType::MainGroupWithOption:
		MenuGroup->SetVisibility(ESlateVisibility::Visible);
		OptionGroup->SetVisibility(ESlateVisibility::Visible);
		break;
	case EVisibleType::None:
	default:
		MenuGroup->SetVisibility(ESlateVisibility::Collapsed);
		OptionGroup->SetVisibility(ESlateVisibility::Collapsed);
		break;
	}
}