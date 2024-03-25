#include "BTService_UpdateByPerception.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"
#include "Kismet/GameplayStatics.h"

UBTService_UpdateByPerception::UBTService_UpdateByPerception()
{
	NodeName = TEXT("Update blackboard by perception");
}

void UBTService_UpdateByPerception::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return;
	
	UAIPerceptionComponent* PerceptionComponent = AIController->GetPawn()->FindComponentByClass<UAIPerceptionComponent>();
	if (!PerceptionComponent) return;
	
	APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!Player) return;
	
	UBlackboardComponent* BlackboardComponent = AIController->GetBlackboardComponent();
	if (!BlackboardComponent) return;

	TArray<AActor*> PerceivedActors;
	PerceptionComponent->GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PerceivedActors);
	if (PerceivedActors.Num() > 0)
	{
		for (AActor* Actor : PerceivedActors)
		{
			if (Actor && Actor->ActorHasTag(FName("Player")))
			{
				BlackboardComponent->SetValueAsVector("PlayerLocation", Actor->GetActorLocation());
				BlackboardComponent->SetValueAsVector("DetectedLocation", Actor->GetActorLocation());
			}
		}
	}
	else
	{
		BlackboardComponent->ClearValue("PlayerLocation");
	}
}
