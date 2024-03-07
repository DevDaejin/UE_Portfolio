#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "EnemyBase.generated.h"

class UWidgetComponent;

UCLASS()
class UE_PORTFOLIO_API AEnemyBase : public ACharacterBase
{
	GENERATED_BODY()
	
public:

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
