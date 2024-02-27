#include "HealthComponent.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	Full();
}

void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

int32 UHealthComponent::GetCurrentHP()
{
	return HP;
}

void UHealthComponent::EarnHP(int Amount)
{
	HP += Amount;

	if (HP > MaxHP)
	{
		Full();
	}
}

bool UHealthComponent::LostHP(int Amount)
{
	HP -= Amount;

	if (HP <= 0)
	{
		Kill();
		return false;
	}

	return true;
}

void UHealthComponent::EarnMaxHP(int Amount)
{
	MaxHP += Amount;
	HP += Amount;
}

void UHealthComponent::LostMaxHP(int Amount)
{
	MaxHP -= Amount;

	if (MaxHP <= 0)
		MaxHP = 1;

	if (MaxHP < HP)
	{
		Full();
	}
}

void UHealthComponent::Kill()
{
	HP = 0;
}

void UHealthComponent::Full()
{
	HP = MaxHP;
}

