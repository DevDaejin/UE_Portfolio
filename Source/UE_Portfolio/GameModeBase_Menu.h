// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameModeBase_Menu.generated.h"

class UUserWidget;

UCLASS()
class UE_PORTFOLIO_API AGameModeBase_Menu : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> MenuWidget;

private:
	const FString MenuName = TEXT("Menu");
	const FString MainName = TEXT("GameMain");
};
