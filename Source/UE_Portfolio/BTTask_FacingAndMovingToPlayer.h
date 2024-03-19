#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_FacingAndMovingToPlayer.generated.h"

UCLASS()
class UE_PORTFOLIO_API UBTTask_FacingAndMovingToPlayer : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTTask_FacingAndMovingToPlayer();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere)
	float RotationSpeed = 500.f;
	UPROPERTY(EditAnywhere)
	float MovementSpeed = 200.f;
};