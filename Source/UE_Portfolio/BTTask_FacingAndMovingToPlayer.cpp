#include "BTTask_FacingAndMovingToPlayer.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTTask_FacingAndMovingToPlayer::UBTTask_FacingAndMovingToPlayer()
{
	NodeName = TEXT("Facing and move to player");
}

EBTNodeResult::Type UBTTask_FacingAndMovingToPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return EBTNodeResult::Failed;
	}

	APawn* Pawn = AIController->GetPawn();
	if (!Pawn)
	{
		return EBTNodeResult::Failed;
	}

	FVector Destination = OwnerComp.GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey());

	FVector Direction = (Destination - Pawn->GetActorLocation()).GetUnsafeNormal();
	FRotator CurrentRotation = Pawn->GetActorRotation();
	FRotator TargetRotation = Direction.Rotation();
	FRotator NewRotation = FRotator(CurrentRotation.Pitch, TargetRotation.Yaw, CurrentRotation.Roll);

	Pawn->SetActorRotation(
		FMath::RInterpTo(
			CurrentRotation,
			NewRotation,
			GetWorld()->GetDeltaSeconds(),
			RotationSpeed));

	AIController->MoveToLocation(Destination, MovementSpeed, true, true, false, true, nullptr, true);
	
	float AcceptanceRadius = 100.0f;
	if (FVector::DistSquared(Pawn->GetActorLocation(), Destination) <= FMath::Square(AcceptanceRadius))
	{
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::InProgress;
}
