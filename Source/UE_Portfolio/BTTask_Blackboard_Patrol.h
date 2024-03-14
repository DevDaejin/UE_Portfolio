#pragma once

#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "CoreMinimal.h"
#include "BTTask_Blackboard_Patrol.generated.h"

/**
 * 
 */
UCLASS()
class UE_PORTFOLIO_API UBTTask_Blackboard_Patrol : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTTask_Blackboard_Patrol();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
