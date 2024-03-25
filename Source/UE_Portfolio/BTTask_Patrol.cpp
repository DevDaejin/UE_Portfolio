#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "EnemyBase.h"
#include "Navigation/PathFollowingComponent.h"
#include "BTTask_Patrol.h"

UBTTask_Patrol::UBTTask_Patrol()
{
	NodeName = TEXT("Patrol");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_Patrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AEnemyBase* EnemyBase = Cast<AEnemyBase>(OwnerComp.GetAIOwner()->GetPawn());
	FVector Destination = OwnerComp.GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey());
	FVector Location1 = EnemyBase->PatrolLocation1;
	FVector Location2 = EnemyBase->PatrolLocation2;

	FVector NextLocation;
	if (Destination == Location1)
	{
		NextLocation = Location2;
	}
	else if (Destination == Location2)
	{
		NextLocation = Location1;
	}
	else
	{
		NextLocation = Location1;
	}

	Destination = NextLocation;
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), Destination);

	bIsMoving = true;
	return EBTNodeResult::InProgress;
}

void UBTTask_Patrol::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (!bIsMoving) return;

	AAIController* AIController = OwnerComp.GetAIOwner();
	APawn* AIPawn = AIController->GetPawn();
	if (!AIPawn) return;

	UBlackboardComponent* Blackboard = AIController->GetBlackboardComponent();
	FVector Destination = Blackboard->GetValueAsVector(GetSelectedBlackboardKey());

	FVector Direction = (Destination - AIPawn->GetActorLocation()).GetSafeNormal();
	FRotator TargetRotator = Direction.Rotation();

	FRotator NewRotation = FMath::RInterpTo(AIPawn->GetActorRotation(), TargetRotator, DeltaSeconds, RotationSpeed);
	NewRotation.Pitch = 0;
	NewRotation.Roll = 0;
	AIPawn->SetActorRotation(NewRotation);

	if (AIController->GetPathFollowingComponent()->GetStatus() != EPathFollowingStatus::Moving)
	{
		AIController->MoveToLocation(Destination, 0);
	}

	float Radius = 100.f;
	float Distance = FVector::DistSquared(AIPawn->GetActorLocation(), Destination);
	float Condition = FMath::Square(Radius);

	if (Distance <= Condition)
	{
		bIsMoving = false;
		AIController->StopMovement();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
