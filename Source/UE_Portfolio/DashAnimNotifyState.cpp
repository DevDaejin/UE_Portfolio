#include "DashAnimNotifyState.h"
#include "HeroPawn.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"

void UDashAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	if (AHeroPawn* Hero = Cast<AHeroPawn>(MeshComp->GetOwner()))
	{
		FVector Direction = Hero->GetActorForwardVector();
		Forward = Hero->GetActorForwardVector();
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
		if (Duriation >= Time)
		{
			Time += FrameDeltaTime;
			FVector Location = Hero->GetActorLocation() + (Forward * Hero->DashSpeed * FrameDeltaTime/Duriation);
			Hero->SetActorLocation(Location);
		}

		if (UCapsuleComponent* Capsule = Cast<UCapsuleComponent>(Hero->GetRootComponent()))
		{
			if (InvincibleStartTime >= Time && InvincibleEndTime <= Time)
			{
				Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			}
			else
			{
				Capsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			}
		}
	}
}
