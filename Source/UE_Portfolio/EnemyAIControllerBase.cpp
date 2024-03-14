#include "EnemyAIControllerBase.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

void AEnemyAIControllerBase::BeginPlay()
{
	Super::BeginPlay();

	if (BehaviorTree && BehaviorTree->BlackboardAsset)
	{
		/*BlackboardComponent->InitializeBlackboard(*(BehaviorTree->BlackboardAsset));
		RunBehaviorTree(BehaviorTree);
		BlackboardComponent->SetValueAsVector("PatrolLocation", FVector(0, 0, 0));*/
	}
}
