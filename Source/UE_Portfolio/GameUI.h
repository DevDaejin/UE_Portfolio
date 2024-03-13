#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameUI.generated.h"

class UImage;
class UProgressBar;

UCLASS()
class UE_PORTFOLIO_API UGameUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* Begin;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* HPBar;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* StaminaBar;

	void PlayEnterdAnimation();
	
	UFUNCTION()
	void UpdateHPBar(float percentage);
	UFUNCTION()
	void UpdateStaminaBar(float percentage);
};
