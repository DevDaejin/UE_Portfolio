#include "HealthComponent.h"
#include "HPBar.h"
#include "Engine/Blueprint.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	//HPBar = CreateWidget<UHPBar>(GetWorld(), HPBarSubclass);
	//if (HPBar)
	//{
	//	UpdateHPBar();
	//}

	//Full();

	FString WidgetPath = "/Game/Blueprints/BP_EnemyHPBar.BP_EnemyHPBar_C";
	UClass* WidgetClass = LoadClass<UUserWidget>(nullptr, *WidgetPath);
	if (WidgetClass)
	{
		// 로드 성공
		UUserWidget* WidgetInstance = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);
		UE_LOG(LogTemp, Display, TEXT("00"));
		if (WidgetInstance)
		{
			UE_LOG(LogTemp, Display, TEXT("11"));
			// 위젯 인스턴스 생성 성공
			// 액터에 위젯 컴포넌트 추가
			UWidgetComponent* WidgetComponent = NewObject<UWidgetComponent>(this);
			if (WidgetComponent)
			{
				// 생성된 위젯 컴포넌트에 위젯 설정
				WidgetComponent->SetWidget(WidgetInstance);

				// 액터에 위젯 컴포넌트 추가
				WidgetComponent->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
				UE_LOG(LogTemp, Display, TEXT("AA"));
			}
			else
			{
				// 위젯 컴포넌트 생성 실패
				UE_LOG(LogTemp, Display, TEXT("CC"));
			}
		}
	}
	else
	{
		// 로드 실패
		UE_LOG(LogTemp, Display, TEXT("BB"));
	}
}

void UHealthComponent::InitializeComponent()
{
	Super::InitializeComponent();
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
	UE_LOG(LogTemp, Display, TEXT("Owner : %s"), *GetOwner()->GetFName().ToString());
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

