#include "CharacterBase.h"
#include "HealthComponent.h"
#include "AttackComponent.h"
#include "Engine/DamageEvents.h"
#include "Components/CapsuleComponent.h"

#include "GameFramework/CharacterMovementComponent.h"

ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	HealthComponent = FindComponentByClass<UHealthComponent>();
	AttackComponent = FindComponentByClass<UAttackComponent>();
}

void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float ACharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (DamageAmount > 0.f)
	{
		UE_LOG(LogTemp, Display, TEXT("D %f"), DamageAmount);

		HealthComponent->LostHP(DamageAmount);

		if (HealthComponent && HealthComponent->GetCurrentHP() == 0)
		{
			FVector Impulse(0, 0, 0);
			if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
			{
				FPointDamageEvent* PointDamageEvent = (FPointDamageEvent*)&DamageEvent;
				Impulse = -(PointDamageEvent->ShotDirection);
			}

			UE_LOG(LogTemp, Display, TEXT("Impulse : %s"), *Impulse.ToString());

			Death(Impulse);
		}
	}
	return 0.0f;
}

void ACharacterBase::Death(FVector ForceDirection)
{
	USkeletalMeshComponent* MeshComponent = GetMesh();
	if (MeshComponent)
	{
		MeshComponent->SetCollisionProfileName(TEXT("Ragdoll"));
		MeshComponent->SetSimulatePhysics(true);
		MeshComponent->SetAllBodiesSimulatePhysics(true);
		MeshComponent->WakeAllRigidBodies();
		MeshComponent->AddImpulse(ForceDirection * 3000.f, TEXT("spine_05"), true);

		UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
		if (MovementComponent)
		{
			MovementComponent->StopMovementImmediately();
			MovementComponent->DisableMovement();
			MovementComponent->SetComponentTickEnabled(false);
		}
	}

	UCapsuleComponent* CapsuleComp = GetComponentByClass<UCapsuleComponent>();
	if (CapsuleComp)
	{
		CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ACharacterBase::Attack()
{
	AttackComponent->Attack();
}
