#include "CharacterBase.h"
#include "HealthComponent.h"
#include "AttackComponent.h"
#include "Engine/DamageEvents.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	AttackComponent = CreateDefaultSubobject<UAttackComponent>(TEXT("Attack"));
	HealthComponen = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));
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

float ACharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (DamageAmount > 0.f &&
		HealthComponen && 
		HealthComponen->GetCurrentHP() > 0)
	{
		HealthComponen->LostHP(DamageAmount);
		FVector Impulse(0, 0, 0);

		if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
		{
			FPointDamageEvent* PointDamageEvent = (FPointDamageEvent*)&DamageEvent;
			Impulse = -(PointDamageEvent->ShotDirection);
		}

		if (HealthComponen->GetCurrentHP() <= 0)
		{
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

void ACharacterBase::NormalAttack()
{
	AttackComponent->NormalAttack();
}
