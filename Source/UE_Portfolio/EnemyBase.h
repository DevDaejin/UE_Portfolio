#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "EnemyBase.generated.h"

class UCameraComponent;
class UWidgetComponent;

UCLASS()
class UE_PORTFOLIO_API AEnemyBase : public ACharacterBase
{
	GENERATED_BODY()
	
public:

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> LockedOnWidgetSubClass;
	UWidgetComponent* WidgetComponent;

	UCameraComponent* Camera;

	bool bLcoked;
	void LockOn(bool bLcoked);
};
