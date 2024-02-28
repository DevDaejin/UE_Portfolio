#include "CharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"

ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACharacterBase::Death()
{
	USkeletalMeshComponent* MeshComponent = GetMesh();

	if (MeshComponent)
	{
		MeshComponent->SetCollisionProfileName(TEXT("Ragdoll"));
		MeshComponent->SetSimulatePhysics(true);
		MeshComponent->SetAllBodiesSimulatePhysics(true);
		MeshComponent->WakeAllRigidBodies();

		UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
		if (MovementComponent)
		{
			MovementComponent->StopMovementImmediately();
			MovementComponent->DisableMovement();
			MovementComponent->SetComponentTickEnabled(false);
		}
	}
}

void ACharacterBase::Attack()
{
}
