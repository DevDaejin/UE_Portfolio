#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameUI.generated.h"

UCLASS()
class UE_PORTFOLIO_API UGameUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidgetAnim))
	UWidgetAnimation* Begin;

	void PlayEnterdAnimation()
	{
		PlayAnimation(Begin);
	}
};
