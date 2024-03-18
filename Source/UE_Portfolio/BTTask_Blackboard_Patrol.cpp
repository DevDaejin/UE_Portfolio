#include "BTTask_Blackboard_Patrol.h"
#include "EnemyAIControllerBase.h"

UBTTask_Blackboard_Patrol::UBTTask_Blackboard_Patrol()
{
	NodeName = "Patrol location";
}

EBTNodeResult::Type UBTTask_Blackboard_Patrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AEnemyAIControllerBase* AIController = Cast<AEnemyAIControllerBase>(OwnerComp.GetAIOwner()))
	{
		
	}

	return EBTNodeResult::Succeeded;
}
