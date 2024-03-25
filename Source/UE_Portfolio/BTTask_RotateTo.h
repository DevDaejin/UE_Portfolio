// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_RotateTo.generated.h"

/**
 * 
 */
UCLASS()
class UE_PORTFOLIO_API UBTTask_RotateTo : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTask_RotateTo();
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere)
	float RotationSpeed = 3.f;
};
