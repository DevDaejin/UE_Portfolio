// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_UpdateDetectedLocation.h"

void UBTService_UpdateDetectedLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UE_LOG(LogTemp, Display, TEXT("AAA"));
}
