#include "DashAnimNotifyState.h"
#include "HeroPawn.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"

void UDashAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	if (AHeroPawn* Hero = Cast<AHeroPawn>(MeshComp->GetOwner()))
	{
		FVector Forward = Hero->GetActorForwardVector() * Hero->MoveDirection.Y;
		FVector Right = Hero->GetActorRightVector() * Hero->MoveDirection.X;
		Direction = (Forward + Right).GetSafeNormal();

		Duriation = Hero->DashMontage->GetPlayLength();
		InvincibleStartTime = Hero->InvincibleStartTime;
		InvincibleEndTime = Hero->InvincibleEndTime;
		Time = 0;
	}
}

void UDashAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (AHeroPawn* Hero = Cast<AHeroPawn>(MeshComp->GetOwner()))
	{
		
	}
}
