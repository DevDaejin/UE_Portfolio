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

	UPROPERTY(meta = (BindWidget))
	UImage* Target;

	void PlayEnterdAnimation();
	UFUNCTION()
	void UpdateHPBar(float percentage);
	void UpdateStaminaBar(float percentage);
	void SetLockOnWidgetPosition(FVector2D& Position);
	void DeactiveTarget();
};
