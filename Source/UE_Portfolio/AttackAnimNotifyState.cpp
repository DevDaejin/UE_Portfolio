#include "AttackAnimNotifyState.h"
#include "Engine.h"
#include "HeroPawn.h"
#include "AttackComponent.h"

void UAttackAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	if (AHeroPawn* Hero = Cast<AHeroPawn>(MeshComp->GetOwner()))
	{
		Hero->AttackComponent->bIsAttacking = true;
	}
}

//void UAttackAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
//{
//	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
//}

void UAttackAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (AHeroPawn* Hero = Cast<AHeroPawn>(MeshComp->GetOwner()))
	{
		Hero->AttackComponent->bIsAttacking = false;
	}
}
