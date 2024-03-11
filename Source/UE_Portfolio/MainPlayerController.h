#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

class UMainUI;
class UGameUI;

UCLASS()
class UE_PORTFOLIO_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> MainUIWidget;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> GameUIWidget;

	UMainUI* MainUI;
	UGameUI* GameUI;

	void ShowMainUI(bool bActive);
	void ShowGameUI(bool bActive);
};
