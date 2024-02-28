#include "AttackComponent.h"
#include "GameFramework/Character.h"
#include "Weapon.h"
#include "HeroPawn.h"
#include "CharacterBase.h"
#include "HealthComponent.h"
#include "GameFramework/DamageType.h"


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

		if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_GameTraceChannel11, Params))
		{
			ACharacterBase* CharacterBase = Cast<ACharacterBase>(HitResult.GetActor()->GetComponentByClass(ACharacterBase::StaticClass()));
			if (CharacterBase != nullptr)
			{
				////TODO : StatComponent도 만들어서 추가하면 될 듯
				//int32 Damage = Weapon->WeaponDamage;
				//APawn* Pawn = Cast<APawn>(GetOwner());
				//CharacterBase->TakeDamage(Damage, FDamageEvent(UDamageType::StaticClass()), Pawn->GetInstigator(), this);
			}
		}
	}
}

