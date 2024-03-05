#include "HealthComponent.h"
#include "HPBar.h"
#include "Blueprint/UserWidget.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	HPBar = CreateWidget<UHPBar>(GetWorld(), HPBarSubclass);
	if (HPBar)
	{
		UpdateHPBar();
	}

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
		return;
	}

	UpdateHPBar();
}

bool UHealthComponent::LostHP(int Amount)
{
	HP -= Amount;

	if (HP <= 0)
	{
		Kill();
		return false;
	}

	UpdateHPBar();
	return true;
}

void UHealthComponent::EarnMaxHP(int Amount)
{
	MaxHP += Amount;
	HP += Amount;

	UpdateHPBar();
}

void UHealthComponent::LostMaxHP(int Amount)
{
	MaxHP -= Amount;

	if (MaxHP <= 0)
		MaxHP = 1;

	if (MaxHP < HP)
	{
		Full();
		return;
	}

	UpdateHPBar();
}

void UHealthComponent::UpdateHPBar()
{
	if (HPBar)
	{
		HPBar->UpdateHPBar(HP / MaxHP);
	}
}

void UHealthComponent::Kill()
{
	HP = 0;
	UpdateHPBar();
}

void UHealthComponent::Full()
{
	HP = MaxHP;
	UpdateHPBar();
}

