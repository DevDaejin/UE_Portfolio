// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_Patrol.generated.h"

/**
 * 
 */
UCLASS()
class UE_PORTFOLIO_API UBTTask_Patrol : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_Patrol();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	bool bIsMoving = false;
	UPROPERTY(EditAnywhere)
	float RotationSpeed = 5.f;
	UPROPERTY(EditAnywhere)
	float MovementSpeed = 100.f;
};
