#include "EnemyAIControllerBase.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

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

void AEnemyAIControllerBase::Tick(float DeltaSeconds)
{
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (LineOfSightTo(PlayerPawn))
	{
		SetFocus(PlayerPawn);
		MoveToActor(PlayerPawn, 200);
	}
	else
	{
		ClearFocus(EAIFocusPriority::Default);
		StopMovement();
	}
}
