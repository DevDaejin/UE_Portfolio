// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainUI.generated.h"

enum class EVisibleType : uint8
{
	MenuGroup,
	MainGroupWithOption,
	Option,
	None
};

class UButton;

UCLASS()
class UE_PORTFOLIO_API UMainUI : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

private:
	APlayerController* PlayerController;
	bool ValidatePlayerController();
	bool ValidateButton(UButton* Button, const FString& ButtonName);

	const FName NewGameLevelName = FName("GameMain");
	const FString Quit = TEXT("quit");

public:
	UPROPERTY(meta = (BindWidget))
	UWidget* MenuGroup;
	UPROPERTY(meta = (BindWidget))
	UWidget* OptionGroup;

	UPROPERTY(meta = (BindWidget))
	UButton* MenuNewButton;
	UPROPERTY(meta = (BindWidget))
	UButton* MenuStartButton;
	UPROPERTY(meta = (BindWidget))
	UButton* MenuOptionButton;
	UPROPERTY(meta = (BindWidget))
	UButton* MenuQuitButton;

	UPROPERTY(meta = (BindWidget))
	UButton* OptionConfirmButton;
	UPROPERTY(meta = (BindWidget))
	UButton* OptionQuitButton;

	UFUNCTION()
	void OnMenuNewButtonClicked();
	UFUNCTION()
	void OnMenuStartButtonClicked();
	UFUNCTION()
	void OnMenuOptionButtonClicked();
	UFUNCTION()
	void OnMenuQuitButtonClicked();

	UFUNCTION()
	void OnOptionConfirmButtonClicked();
	UFUNCTION()
	void OnOptionQuitButtonClicked();

	void SetGroupVisibility(EVisibleType type);
}; 