#include "EnemyBase.h"
#include "HeroPawn.h"
#include "HealthComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/WidgetComponent.h"


void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	UClass* WidgetClass = LockedOnWidgetSubClass;
	if (WidgetClass)
	{
		UUserWidget* WidgetInstance = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);
		if (WidgetInstance)
		{
			WidgetComponent = NewObject<UWidgetComponent>(GetOwner());
			if (WidgetComponent)
			{
				WidgetComponent->SetWidget(WidgetInstance);

				WidgetComponent->RegisterComponent();
				WidgetComponent->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
				WidgetComponent->SetRelativeLocation(FVector(0, 0, 0));
			}
		}
	}

	Camera = 
}

void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bLcoked)
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
}

void AEnemyBase::LockOn(bool bLcoked)
{
	this->bLcoked = bLcoked;
}

