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
	UPROPERTY(EditAnywhere)
	FVector PatrolLocation1;
	UPROPERTY(EditAnywhere)
	FVector PatrolLocation2;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
