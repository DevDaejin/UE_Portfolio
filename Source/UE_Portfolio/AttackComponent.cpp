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
	Arm();
}

void UAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (bIsAttacking)
	{
		CheckWeaponCollision();
	}
}

void UAttackComponent::Arm()
{
	APawn* Pawn = Cast<APawn>(GetOwner());
	if (WeaponSubclass && Pawn)
	{
		FActorSpawnParameters Params;
		Params.Owner = Pawn;
		Params.Instigator = Pawn;

		Weapon = GetWorld()->SpawnActor<AWeapon>(WeaponSubclass, FTransform::Identity, Params);
		ACharacter* Character = Cast<ACharacter>(Pawn);
		if (Weapon && Character)
		{
			Weapon->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("weapon_r_muzzle"));
		}
	}
}

void UAttackComponent::Attack()
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());

	if (Character)
	{
		Enemies.Empty();

		UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
		if (AnimInstance && AttackMontage && !bIsAttacking)
		{
			AnimInstance->Montage_Play(AttackMontage, 1.f);
		}
	}
}

void UAttackComponent::CheckWeaponCollision()
{
	if (Weapon)
	{
		FVector Start = Weapon->MeshComponent->GetSocketLocation("StartPoint");
		FVector End = Weapon->MeshComponent->GetSocketLocation("EndPoint");

		FHitResult HitResult;
		FCollisionQueryParams Params;

		if(GetOwner())
		{
			Params.AddIgnoredActor(GetOwner());
		}
		Params.AddIgnoredActor(Weapon);

		bool bHit = HitResult.bBlockingHit;
		FColor LineColor = bHit ? FColor::Green : FColor::Red;
		float LineTime = 2.0f;
		DrawDebugLine(GetWorld(), Start, End, LineColor, false, LineTime);

		if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params))
		{
			ACharacterBase* CharacterBase = Cast<ACharacterBase>(HitResult.GetActor());
			if (CharacterBase && !Enemies.Contains(CharacterBase))
			{
				UE_LOG(LogTemp, Display, TEXT("CharacterBase %s"), *CharacterBase->GetFName().ToString());
				float Damage = Weapon->WeaponDamage;
				FPointDamageEvent DamageEvent;
				DamageEvent.HitInfo = HitResult;
				DamageEvent.ShotDirection = (End - Start).GetSafeNormal();
				DamageEvent.Damage = Damage;

				CharacterBase->TakeDamage(Damage, DamageEvent, GetOwner()->GetInstigatorController(), Weapon);
				Enemies.Add(CharacterBase);
			}
		}
	}
}

