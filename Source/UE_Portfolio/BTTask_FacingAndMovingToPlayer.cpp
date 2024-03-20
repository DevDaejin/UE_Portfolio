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
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController* AIController = OwnerComp.GetAIOwner();
	FVector AILocation = AIController->GetNavAgentLocation();
	FVector TargetLocation =
		OwnerComp.GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey());
	FRotator TargetRotation = (TargetLocation - AILocation).Rotation();
	TargetRotation.Pitch = 0;
	TargetRotation.Roll = 0;

	if (AIController->GetControlRotation() == TargetRotation)
	{
		return EBTNodeResult::Succeeded;
	}
	else
	{
		FRotator NewRotation = FMath::RInterpTo(
			AIController->GetControlRotation(), 
			TargetRotation, 
			GetWorld()->GetDeltaSeconds(), 
			RotationSpeed);

		UE_LOG(LogTemp, Display, TEXT("ai rot : %s"), *AIController->GetControlRotation().ToString());
		UE_LOG(LogTemp, Display, TEXT("tar rot : %s"), *TargetRotation.ToString());

		AIController->GetPawn()->SetActorRotation(NewRotation.Quaternion());

		return EBTNodeResult::InProgress;
	}
}
