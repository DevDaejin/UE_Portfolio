#include "Weapon.h"
#include "Components/BoxComponent.h"

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	MeshComponent = FindComponentByClass<USkeletalMeshComponent>();
}

