#include "HealthComponent.h"
#include "HPBar.h"
#include "Engine/Blueprint.h"
#include "Blueprint/UserWidget.h"
#include "HeroPawn.h"
#include "Camera/CameraComponent.h"
#include "Components/WidgetComponent.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	UClass* WidgetClass = HPBarSubclassOf;
	if (WidgetClass)
	{
		UUserWidget* WidgetInstance = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);
		if (WidgetInstance)
		{
			HPBar = Cast<UHPBar>(WidgetInstance);
			WidgetComponent = NewObject<UWidgetComponent>(GetOwner());
			if (WidgetComponent)
			{
				WidgetComponent->SetWidget(WidgetInstance);

				WidgetComponent->RegisterComponent();
				WidgetComponent->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
				WidgetComponent->SetRelativeLocation(FVector(0, 0, 130));
			}
		}
	}

	Full();
}

void UHealthComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	Billboarding();
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

void UHealthComponent::Billboarding()
{
	if (!Camera)
	{
		if (GetWorld() &&
			GetWorld()->GetFirstPlayerController() &&
			GetWorld()->GetFirstPlayerController()->GetPawn())
		{
			AHeroPawn* HeroPawn = Cast<AHeroPawn>(GetWorld()->GetFirstPlayerController()->GetPawn());
			if (HeroPawn)
			{
				Camera = HeroPawn->Camera;
			}
		}
	}

	if (Camera && WidgetComponent)
	{
		FVector CameraLocation = Camera->GetComponentLocation();
		FRotator LookAtRotation = (CameraLocation - WidgetComponent->GetComponentLocation()).Rotation();
		WidgetComponent->SetWorldRotation(FRotator(0, LookAtRotation.Yaw, 0));
	}
}

