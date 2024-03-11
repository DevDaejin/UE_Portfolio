#include "MainPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "GameUI.h"
//#include "MainUI.h"

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//if (MainUIWidget && !MainUI)
	//{
	//	MainUI = CreateWidget<UMainUI>(this, MainUIWidget);
	//	if (MainUI)
	//	{
	//		MainUI->AddToViewport();
	//	}
	//}

	if (GameUIWidget && !GameUI)
	{
		GameUI = CreateWidget<UGameUI>(this, GameUIWidget);
		if (GameUI)
		{
			GameUI->AddToViewport();
		}
	}

	ShowGameUI(true);
	GameUI->PlayEnterdAnimation();
}
void AMainPlayerController::ShowMainUI(bool bActive)
{
	//if (MainUI)
	//{
	//	MainUI->SetVisibility(
	//		bActive ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	//}
}

void AMainPlayerController::ShowGameUI(bool bActive)
{
	if (GameUI)
	{
		GameUI->SetVisibility(
			bActive ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

		if(GameUI->GetVisibility() == ESlateVisibility::Hidden)
			UE_LOG(LogTemp, Display, TEXT("HIdden"));

		if (GameUI->GetVisibility() == ESlateVisibility::Visible)
			UE_LOG(LogTemp, Display, TEXT("Visible"));
	}
}
