#include "AttackComponent.h"
#include "Weapon.h"
#include "HeroPawn.h"
#include "CharacterBase.h"
#include "HealthComponent.h"
#include "GameFramework/DamageType.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"

UAttackComponent::UAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAttackComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (bIsAttacking)
	{
		CheckWeaponCollision();
	}
}

void UAttackComponent::Attack()
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (Character)
	{
		Enemies.Empty();
		OldFVectors.Empty();
		CurrentFVectors.Empty();
			
		UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
		if (AnimInstance && AttackMontage && !bIsAttacking)
		{
			AnimInstance->Montage_Play(AttackMontage);
		}
	}
}

void UAttackComponent::CheckWeaponCollision()
{
	if (!CurrentFVectors.IsEmpty())
	{
		OldFVectors = CurrentFVectors;
	}

	CurrentFVectors.Empty();

	if (WeaponTraceSegment < 2)
	{
		WeaponTraceSegment = 2;
	}

	USkeletalMeshComponent* MeshComponent = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();

	FVector Start = MeshComponent->GetSocketLocation("FX_Sword_HiltBase");
	FVector End = MeshComponent->GetSocketLocation("FX_Sword_BeyondTip");

	for (size_t i = 0; i < WeaponTraceSegment; i++)
	{
		CurrentFVectors.Add(FMath::Lerp(Start, End, (float)i / (float)WeaponTraceSegment));
	}

	if (OldFVectors.IsEmpty())
	{
		OldFVectors = CurrentFVectors;
	}

	TArray<FHitResult> HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());

	for (size_t i = 0; i < WeaponTraceSegment; i++)
	{
		DrawDebugLine(GetWorld(), OldFVectors[i], CurrentFVectors[i], FColor::Red, false, 2.0f);

		if (GetWorld()->LineTraceMultiByChannel(HitResult, OldFVectors[i], CurrentFVectors[i], ECollisionChannel::ECC_GameTraceChannel2, Params))
		{
			for (const FHitResult Hitted : HitResult)
			{
				ACharacterBase* CharacterBase = Cast<ACharacterBase>(Hitted.GetActor());
				if (CharacterBase && !Enemies.Contains(CharacterBase))
				{
					UE_LOG(LogTemp, Display, TEXT("CharacterBase %s"), *CharacterBase->GetFName().ToString());
					FPointDamageEvent DamageEvent;
					DamageEvent.HitInfo = Hitted;
					DamageEvent.ShotDirection = (End - Start).GetSafeNormal();
					DamageEvent.Damage = Damage;

					CharacterBase->TakeDamage(Damage, DamageEvent, GetOwner()->GetInstigatorController(), GetOwner());
					Enemies.Add(CharacterBase);
				}
			}
		}
	}
}

