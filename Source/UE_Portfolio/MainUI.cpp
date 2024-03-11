// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUI.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UMainUI::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerController = GetWorld()->GetFirstPlayerController();

	if (ValidatePlayerController())
	{
		PlayerController->bShowMouseCursor = true;
	}

	if (ValidateButton(MenuNewButton, TEXT("MenuQuitButton")))
	{
		MenuNewButton->OnClicked.AddDynamic(this, &UMainUI::OnMenuNewButtonClicked);
	}

	if (ValidateButton(MenuStartButton, TEXT("MenuStartButton")))
	{//추 후 이전 게임 Load로
		MenuStartButton->OnClicked.AddDynamic(this, &UMainUI::OnMenuStartButtonClicked);
	}

	if (ValidateButton(MenuOptionButton, TEXT("MenuOptionButton")))
	{
		MenuOptionButton->OnClicked.AddDynamic(this, &UMainUI::OnMenuOptionButtonClicked);
	}

	if (ValidateButton(MenuQuitButton, TEXT("MenuQuitButton")))
	{
		MenuQuitButton->OnClicked.AddDynamic(this, &UMainUI::OnMenuQuitButtonClicked);
	}

	if (ValidateButton(OptionQuitButton, TEXT("OptionQuitButton")))
	{
		OptionQuitButton->OnClicked.AddDynamic(this, &UMainUI::OnOptionQuitButtonClicked);
	}
}

bool UMainUI::ValidatePlayerController()
{
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Error, TEXT("Player controller is null"));
		return false;
	}
	return true;
}

bool UMainUI::ValidateButton(UButton* Button, const FString& ButtonName)
{
	if (!Button)
	{
		UE_LOG(LogTemp, Error, TEXT("%s is null"), *ButtonName);
		return false;
	}
	return true;
}

void UMainUI::OnMenuNewButtonClicked()
{
	if (ValidatePlayerController())
	{
		UGameplayStatics::OpenLevel(this, NewGameLevelName);
	}
}

void UMainUI::OnMenuStartButtonClicked()
{
	if (ValidatePlayerController())
	{
		UGameplayStatics::OpenLevel(this, NewGameLevelName);
	}
}

void UMainUI::OnMenuOptionButtonClicked()
{
	SetGroupVisibility(EVisibleType::MainGroupWithOption);
}

void UMainUI::OnMenuQuitButtonClicked()
{
	if (ValidatePlayerController())
	{
		PlayerController->ConsoleCommand(Quit);
	}
}

void UMainUI::OnOptionConfirmButtonClicked()
{
	SetGroupVisibility(EVisibleType::MenuGroup);
}

void UMainUI::OnOptionQuitButtonClicked()
{
	SetGroupVisibility(EVisibleType::MenuGroup);
}

void UMainUI::SetGroupVisibility(EVisibleType type)
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