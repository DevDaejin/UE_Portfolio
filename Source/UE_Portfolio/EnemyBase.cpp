#include "EnemyBase.h"
#include "HeroPawn.h"
#include "Camera/CameraComponent.h"
#include "Components/WidgetComponent.h"


void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Billboarding();
}

void AEnemyBase::Billboarding()
{
	if (HPWidget && TargetWidget)
	{
		 AHeroPawn* HeroPawn = Cast<AHeroPawn>(GetWorld()->GetFirstPlayerController());
		if (HeroPawn && HeroPawn->Camera)
		{
			UCameraComponent* Camera = HeroPawn->Camera;
			FVector CameraLocation = Camera->GetComponentLocation();
			FRotator LookAtRotation = (CameraLocation - GetActorLocation()).Rotation();

			HPWidget->SetWorldRotation(FRotator(0, LookAtRotation.Yaw, 0));
			TargetWidget->SetWorldRotation(FRotator(0, LookAtRotation.Yaw, 0));
		}
	}
}
