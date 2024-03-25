#include "BTTask_SetPatrolLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "EnemyBase.h"

UBTTask_SetPatrolLocation::UBTTask_SetPatrolLocation()
{
	NodeName = TEXT("Set destination");
}

EBTNodeResult::Type UBTTask_SetPatrolLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard) return EBTNodeResult::Failed;

	AEnemyBase* EnemyBase = Cast<AEnemyBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (!EnemyBase) return EBTNodeResult::Failed;

	FVector Location1 = EnemyBase->PatrolLocation1;
	FVector Location2 = EnemyBase->PatrolLocation2;

	FVector Destination = Blackboard->GetValueAsVector(GetSelectedBlackboardKey());

	if (Destination != Location1 &&
		Destination != Location2)
	{
		Destination = Location1;
	}
	else
	{
		Destination = Destination == Location1 ? Location2 : Location1;
	}

	Blackboard->SetValueAsVector(GetSelectedBlackboardKey(), Destination);

	return EBTNodeResult::Succeeded;
}
