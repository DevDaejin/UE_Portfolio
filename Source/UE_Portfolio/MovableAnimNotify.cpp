#include "MovableAnimNotify.h"
#include "HeroPawn.h"

void UMovableAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (AHeroPawn* Hero = Cast<AHeroPawn>(MeshComp->GetOwner()))
	{
		Hero->bCanInput = true;
	}
}
