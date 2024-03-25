#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_FacingAndMovingToVector.generated.h"

UCLASS()
class UE_PORTFOLIO_API UBTTask_FacingAndMovingToVector : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTTask_FacingAndMovingToVector();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	bool bIsMoving = false;
	UPROPERTY(EditAnywhere)
	float RotationSpeed = 3.f;
	UPROPERTY(EditAnywhere)
	float MovementSpeed = 150.f;
};