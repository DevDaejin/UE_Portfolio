#include "EnemyAIControllerBase.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EnemyBase.h"

void AEnemyAIControllerBase::BeginPlay()
{
	Super::BeginPlay();

	if (BehaviorTree)
	{
		RunBehaviorTree(BehaviorTree);

		if (GetBlackboardComponent())
		{
			AEnemyBase* EnemyBase = Cast<AEnemyBase>(GetPawn());
			if (EnemyBase)
			{
				GetBlackboardComponent()->SetValueAsVector(TEXT("PatrolLocation1"), EnemyBase->PatrolPosition1);
				GetBlackboardComponent()->SetValueAsVector(TEXT("PatrolLocation2"), EnemyBase->PatrolPosition2);
			}
		}
	}
}

void AEnemyAIControllerBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	RunBehaviorTree(BehaviorTree);

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (LineOfSightTo(PlayerPawn))
	{
		GetBlackboardComponent()->SetValueAsVector(TEXT("DetectedLocation"), PlayerPawn->GetActorLocation());
		GetBlackboardComponent()->SetValueAsVector(TEXT("HeroPawnLocation"), PlayerPawn->GetActorLocation());
	}
	else
	{
		GetBlackboardComponent()->ClearValue(TEXT("HeroPawnLocation"));
	}
}
