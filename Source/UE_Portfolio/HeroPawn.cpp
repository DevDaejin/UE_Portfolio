#include "HeroPawn.h"
#include "HealthComponent.h"
#include "AttackComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "DrawDebugHelpers.h"
#include "HealthComponent.h"
#include "AttackComponent.h"
#include "Kismet\KismetSystemLibrary.h"

AHeroPawn::AHeroPawn()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AHeroPawn::BeginPlay()
{
	Super::BeginPlay();

	SpringArm = FindComponentByClass<USpringArmComponent>();
	SpringArmOriginRotation = SpringArm->GetRelativeRotation();

	Camera = FindComponentByClass<UCameraComponent>();
	AnimInstance = GetMesh()->GetAnimInstance();

	SetInputSubsystem();

	CurrentStamina = MaxStamina;
}

void AHeroPawn::Jump()
{
	if (!bLockMovement)
	{
		if (CurrentJumpCount == 0 && CurrentStamina >= JumpStaminaCost)
		{
			LostStamina(JumpStaminaCost);

			Super::Jump();
		}
		else if (CurrentJumpCount == 1 && CurrentStamina >= JumpStaminaCost)
		{
			LostStamina(JumpStaminaCost);

			FVector Direction = GetActorUpVector();
			LaunchCharacter(Direction * SecondJumpForce, false, true);
		}
		else
		{
			return;
		}

		CurrentJumpCount++;
	}
}

void AHeroPawn::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	CurrentJumpCount = 0;
}

void AHeroPawn::LostStamina(float amount)
{
	CurrentStamina -= amount;

	if (CurrentStamina < 0)
	{
		CurrentStamina = 0;
	}

	bChargeStamina = false;
	GetWorld()->GetTimerManager().SetTimer(
		StaminaChargeTimeHandle,
		this,
		&AHeroPawn::ChargeStamina,
		RechargeTime);
}

void AHeroPawn::EarnStamina(float amount)
{
	CurrentStamina += amount;

	if (CurrentStamina > MaxStamina)
	{
		CurrentStamina = MaxStamina;
	}
}

void AHeroPawn::Attack()
{
	//Super::Attack();

	if (AttackComponent &&
		!bLockMovement &&
		CurrentJumpCount == 0 &&
		CurrentStamina >= AttackStaminaCost)
	{
		bLockMovement = true;
		LostStamina(AttackStaminaCost);
		AttackComponent->Attack();
	}
}

float AHeroPawn::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	return 0.0f;
}

void AHeroPawn::Dash()
{
	if (!bLockMovement &&
		CurrentJumpCount == 0 &&
		CurrentStamina >= DashStaminaCost)
	{
		bLockMovement = true;
		LostStamina(DashStaminaCost);
		FVector Direction = Controller->GetPawn()->GetActorForwardVector();
		AnimInstance->Montage_Play(DashMontage, DashMontage->GetPlayLength());
	}
}

void AHeroPawn::ChargeStamina()
{
	bChargeStamina = true;
}

void AHeroPawn::Move(const FInputActionInstance& Instance)
{
	MoveDirection = FVector(
		Instance.GetValue().Get<FVector2D>().GetSafeNormal().X,
		Instance.GetValue().Get<FVector2D>().GetSafeNormal().Y,
		0);

	if (!bLockMovement && Controller)
	{
		FVector ForwardDirection;
		FVector RightDirection;
		FVector Direction;

		if (!LockedOnTarget)
		{
			FRotator CameraRotator = SpringArm->GetRelativeRotation();
			CameraRotator.Pitch = 0;
			CameraRotator.Roll = 0;

			ForwardDirection = FRotationMatrix(CameraRotator).GetUnitAxis(EAxis::X);
			RightDirection = FRotationMatrix(CameraRotator).GetUnitAxis(EAxis::Y);
			Direction = ForwardDirection * MoveDirection.Y + RightDirection * MoveDirection.X;
			if (!Direction.IsNearlyZero())
			{
				FRotator CalculatedRotation = FMath::RInterpTo(
					Controller->GetControlRotation(),
					Direction.Rotation(),
					UGameplayStatics::GetWorldDeltaSeconds(this), RotateSpeed);

				Controller->SetControlRotation(CalculatedRotation);
			}

			AddMovementInput(Direction.GetSafeNormal(), 1);
		}
		else
		{
			ForwardDirection = FRotationMatrix(GetActorRotation()).GetUnitAxis(EAxis::X);
			RightDirection = FRotationMatrix(GetActorRotation()).GetUnitAxis(EAxis::Y);
			Direction = ForwardDirection * MoveDirection.Y + RightDirection * MoveDirection.X;

			if(MoveDirection.Y > 0)
				AnimInstance->Montage_Play(LockOnForwardMontage, LockOnForwardMontage->GetPlayLength());

			if (MoveDirection.Y < 0)
				AnimInstance->Montage_Play(LockOnBackwardMontage, LockOnBackwardMontage->GetPlayLength());

			if (MoveDirection.X > 0)
				AnimInstance->Montage_Play(LockOnRightwardMontage, LockOnRightwardMontage->GetPlayLength());

			if (MoveDirection.X < 0)
				AnimInstance->Montage_Play(LockOnLeftwardMontage, LockOnLeftwardMontage->GetPlayLength());

			if (MoveDirection.X == 0 && MoveDirection.Y == 0)
				AnimInstance->Montage_Stop(0.2f);

			AddMovementInput(Direction.GetSafeNormal(), 0.5f);
		}
	}
}

void AHeroPawn::Look(const FInputActionInstance& Instance)
{
	if (!LockedOnTarget)
	{
		FVector2D LookVector = Instance.GetValue().Get<FVector2D>();
		LookVector.X *= (UGameplayStatics::GetWorldDeltaSeconds(this) * LookingSenstive);
		LookVector.Y *= (UGameplayStatics::GetWorldDeltaSeconds(this) * LookingSenstive);


		FRotator CalculatedSpringArmRotation;

		CalculatedSpringArmRotation =
			SpringArm->GetComponentRotation() + FRotator(-LookVector.Y, LookVector.X, 0.0f);

		CalculatedSpringArmRotation.Pitch = FMath::Clamp(
			CalculatedSpringArmRotation.Pitch,
			MinPitchAngle,
			MaxPitchAngle);

		CalculatedSpringArmRotation.Yaw = FRotator::ClampAxis(CalculatedSpringArmRotation.Yaw);

		SpringArm->SetWorldRotation(CalculatedSpringArmRotation);
	}
}

void AHeroPawn::LockOnTarget(const FInputActionInstance& Instance)
{
	if (LockedOnTarget)
	{
		LockedOnTarget = nullptr;
	}
	else
	{
		TArray<FHitResult> HitResult;

		bool bHitted = DetectEnemy(HitResult);

		if (bHitted && HitResult.Num() > 0)
		{
			LockedOnTarget = HitResult[0].GetActor();
			UE_LOG(LogTemp, Display, TEXT("e %s"), *LockedOnTarget->GetFName().ToString());
		}
	}
}

void AHeroPawn::ChangeLockOn(const FInputActionInstance& Instance)
{
	if (LockedOnTarget)
	{
		TArray<FHitResult> HitResult;

		bool bHitted = DetectEnemy(HitResult);

		UE_LOG(LogTemp, Display, TEXT("NUM : %d"), HitResult.Num());

		if (bHitted && HitResult.Num() > 0)
		{
			for (FHitResult Result : HitResult)
			{
				UE_LOG(LogTemp, Display, TEXT("Elemnt : %s"), *Result.GetActor()->GetFName().ToString());
			}

			int32 Index = (Instance.GetValue().Get<FVector>().X > 0) ? 1 : (HitResult.Num() - 1);

			if (Index < 0)
				Index = 0;

			if (Index >= HitResult.Num())
				Index = HitResult.Num() - 1;

			LockedOnTarget = HitResult[Index].GetActor();

			UE_LOG(LogTemp, Display, TEXT("Index : %d Curret Target : %s"), Index, *LockedOnTarget->GetFName().ToString());
		}
	}
}

bool AHeroPawn::DetectEnemy(TArray<FHitResult>& HitResult)
{
	FVector SphereStart = GetActorLocation();
	FVector SphereEnd = SphereStart + (GetActorForwardVector() * DetectDistance);

	FCollisionShape Sphere = FCollisionShape::MakeSphere(DetectRadius);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	//FVector TraceDistance = GetActorForwardVector() * DetectDistance;

	//DrawDebugSphere(  
	//	GetWorld(),
	//	SphereStart,
	//	10,
	//	32,
	//	FColor::Yellow,
	//	false,
	//	3);

	//DrawDebugSphere(
	//	GetWorld(),
	//	SphereEnd,
	//	10,
	//	32,
	//	FColor::Yellow,
	//	false,
	//	3);

	//DrawDebugCapsule(
	//	GetWorld(),
	//	SphereStart + (TraceDistance * 0.5f),
	//	DetectDistance * 0.5f + DetectRadius,
	//	DetectRadius,
	//	FRotationMatrix::MakeFromZ(TraceDistance).ToQuat(),
	//	FColor::Green,
	//	false,
	//	3.f);

	return GetWorld()->SweepMultiByChannel(
		HitResult,
		SphereStart,
		SphereEnd,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel1,
		Sphere,
		Params);
}


void AHeroPawn::SetInputSubsystem()
{
	APlayerController* PlayerController = Cast<APlayerController>(Controller);

	if (PlayerController)
	{
		PlayerController->bShowMouseCursor = false;

		ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
		if (LocalPlayer)
		{
			UEnhancedInputLocalPlayerSubsystem* Subsystem =
				LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

			if (Subsystem)
			{
				Subsystem->AddMappingContext(CommonMappingContext, 0);
			}
		}
	}
}

void AHeroPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bChargeStamina)
	{
		EarnStamina(StaminaChargeAmount * DeltaTime);
	}

	if (LockedOnTarget)
	{
		FRotator LookAt = (LockedOnTarget->GetActorLocation() - GetActorLocation()).Rotation();
		Controller->SetControlRotation(LookAt);

		FRotator Smooth = FMath::RInterpTo(
			SpringArm->GetRelativeRotation(), 
			Controller->GetControlRotation() + SpringArmOriginRotation, 
			DeltaTime, 
			CamSpeedByLockOn);
		SpringArm->SetRelativeRotation(Smooth);
	}
}

void AHeroPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (Input)
	{
		Input->BindAction(IA_Jump, ETriggerEvent::Started, this, &AHeroPawn::Jump);
		Input->BindAction(IA_Dash, ETriggerEvent::Started, this, &AHeroPawn::Dash);
		Input->BindAction(IA_Movement, ETriggerEvent::Triggered, this, &AHeroPawn::Move);
		Input->BindAction(IA_Looking, ETriggerEvent::Triggered, this, &AHeroPawn::Look);
		Input->BindAction(IA_Attack, ETriggerEvent::Started, this, &AHeroPawn::Attack);
		Input->BindAction(IA_LockOn, ETriggerEvent::Started, this, &AHeroPawn::LockOnTarget);
		Input->BindAction(IA_ChangeLockOn, ETriggerEvent::Started, this, &AHeroPawn::ChangeLockOn);
	}
}