#include "BTTask_FacingAndMovingToVector.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "EnemyBase.h"
#include "Navigation/PathFollowingComponent.h"


UBTTask_FacingAndMovingToVector::UBTTask_FacingAndMovingToVector()
{
	NodeName = TEXT("Facing and move to location");
	//bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_FacingAndMovingToVector::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	bIsMoving = true;
	return EBTNodeResult::InProgress;
}

void UBTTask_FacingAndMovingToVector::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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
}
 