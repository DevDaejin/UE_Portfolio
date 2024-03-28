#include "PlayerAnimInstance.h"
#include "HeroPawn.h"
#include "Kismet/KismetMathLibrary.h"

//Begin
void UPlayerAnimInstance::NativeInitializeAnimation()
{
	HeroPawn = Cast<AHeroPawn>(TryGetPawnOwner());
}

//Tick
void UPlayerAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
	if (!HeroPawn)
	{
		HeroPawn = Cast<AHeroPawn>(TryGetPawnOwner());
	}

	if (!HeroPawn) return;

	FVector Velocity = HeroPawn->GetVelocity();
	Velocity.Z = 0;
	Speed = Velocity.Size();

	FRotator PlayerAimRotation = HeroPawn->GetBaseAimRotation();
	FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(HeroPawn->GetVelocity());

	if (HeroPawn->LockedOnTarget != nullptr)
	{
		MovementOffsetYaw = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, PlayerAimRotation).Yaw;
	}
	else
	{
		MovementOffsetYaw = 0;
	}
}
