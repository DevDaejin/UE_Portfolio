#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIControllerBase.generated.h"

class UBehaviorTree;

UCLASS()
class UE_PORTFOLIO_API AEnemyAIControllerBase : public AAIController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBehaviorTree* BehaviorTree;
	UBlackboardComponent* BlackboardComponent;
};
