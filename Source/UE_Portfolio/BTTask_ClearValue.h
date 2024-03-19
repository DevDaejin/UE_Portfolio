#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_ClearValue.generated.h"

UCLASS()
class UE_PORTFOLIO_API UBTTask_ClearValue : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public :
	UBTTask_ClearValue();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
