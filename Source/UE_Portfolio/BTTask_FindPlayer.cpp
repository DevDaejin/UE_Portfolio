#include "BTTask_FindPlayer.h"
#include "Perception/AIPerceptionComponent.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_FindPlayer::UBTTask_FindPlayer()
{
	NodeName = TEXT("Set dectedLocation");
}

EBTNodeResult::Type UBTTask_FindPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	//AAIController* AIController = OwnerComp.GetAIOwner();
	//if (!AIController) return EBTNodeResult::Failed;

	//UAIPerceptionComponent* AIPerceptionComponent = 
	//	AIController->FindComponentByClass<UAIPerceptionComponent>();
	//if (!AIPerceptionComponent) return EBTNodeResult::Failed;

	//APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	//if (!Player) return EBTNodeResult::Failed;

	//FActorPerceptionBlueprintInfo Info;
	//AIPerceptionComponent->GetActorsPerception(Player, Info);

	//for (const FAIStimulus& Stimulus : Info.LastSensedStimuli)
	//{
	//	if (Stimulus.WasSuccessfullySensed())
	//	{
	//		UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
	//		BlackboardComp->SetValueAsVector(GetSelectedBlackboardKey(), Stimulus.StimulusLocation);
	//		return EBTNodeResult::Succeeded;
	//	}
	//}

	//return EBTNodeResult::Failed;

	return EBTNodeResult::Succeeded;
}
