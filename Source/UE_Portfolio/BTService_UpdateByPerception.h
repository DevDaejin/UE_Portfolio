// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_UpdateByPerception.generated.h"

/**
 * 
 */
UCLASS()
class UE_PORTFOLIO_API UBTService_UpdateByPerception : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTService_UpdateByPerception();
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
