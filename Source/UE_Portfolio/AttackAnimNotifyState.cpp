#include "AttackAnimNotifyState.h"
#include "Engine.h"
#include "CharacterBase.h"
#include "AttackComponent.h"

void UAttackAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	if (ACharacterBase* CharacterBase = Cast<ACharacterBase>(MeshComp->GetOwner()))
	{
		CharacterBase->AttackComponent->SetAttackMode(true, DamageMultiplier);
	}
}

//void UAttackAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
//{
//	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
//}

void UAttackAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (ACharacterBase* CharacterBase = Cast<ACharacterBase>(MeshComp->GetOwner()))
	{
		CharacterBase->AttackComponent->SetAttackMode(false, DamageMultiplier);
	}
}
