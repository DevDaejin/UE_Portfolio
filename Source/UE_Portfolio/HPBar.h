#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HPBar.generated.h"

class UProgressBar;

UCLASS()
class UE_PORTFOLIO_API UHPBar : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HPBar;

	void UpdateHPBar(float percentage);
	void SetHpBarVisible(bool bIsAct);
	void AlwaysVisible(bool bIsAct);

private:
	FTimerHandle VisibleUpdateTimerHandle;
	float Duriation = 1;
	bool bAlwaysVisible;
};
