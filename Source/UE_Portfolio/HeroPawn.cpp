#include "HeroPawn.h"
#include "HealthComponent.h"
#include "AttackComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
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
	OriginMeshYaw = GetMesh()->GetRelativeRotation().Yaw;
}

void AHeroPawn::Jump()
{
	if (bCanInput)
	{
		if (CurrentJumpCount == 0 && CurrentStamina >= JumpStaminaCost)
		{
			Super::Jump();
		}
		else if (CurrentJumpCount == 1 && CurrentStamina >= JumpStaminaCost)
		{
			LaunchCharacter(GetActorUpVector() * SecondJumpForce, false, true);
		}
		else
		{
			return;
		}

		LostStamina(JumpStaminaCost);
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
		bCanInput &&
		CurrentJumpCount == 0 &&
		CurrentStamina >= AttackStaminaCost)
	{
		bCanInput = false;
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
	if (bCanDashing &&
		bCanInput &&
		CurrentJumpCount == 0 &&
		CurrentStamina >= DashStaminaCost)
	{
		bCanInput = false;
		bCanDashing = false;

		DashDirection = MoveDirection;
		DashTickTimer = 0;

		LostStamina(DashStaminaCost);

		GetWorld()->GetTimerManager().SetTimer(
			DashCooldownTimeHanlde, 
			this,
			&AHeroPawn::ResetDashing,
			DashMontage->GetPlayLength());

		AnimInstance->Montage_Play(DashMontage, DashMontage->GetPlayLength());
	}
}

void AHeroPawn::DashTick(float DeltaTime)
{
	float Duriation = (DashMontage->GetPlayLength() * 0.85f);

	if (DashTickTimer < Duriation)
	{
		FVector Location;
		if (LockedOnTarget)
		{
			FVector Direction = 
				GetActorForwardVector() * DashDirection.Y +
				GetActorRightVector() * DashDirection.X;

			Location = 
				GetActorLocation() + 
				(Direction * DashSpeed * DeltaTime / Duriation);
		}
		else
		{
			Location = 
				GetActorLocation() + 
				(GetActorForwardVector() * DashSpeed * DeltaTime / Duriation);
		}

		SetActorLocation(Location);
		DashTickTimer += DeltaTime;
	}

	if (UCapsuleComponent* Capsule = Cast<UCapsuleComponent>(GetRootComponent()))
	{
		if (InvincibleStartTime >= DashTickTimer && InvincibleEndTime <= DashTickTimer)
		{
			Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
		else
		{
			Capsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		}
	}
}

void AHeroPawn::ResetDashing()
{
	bCanDashing = true;	
	GetMesh()->SetRelativeRotation(FRotator(0, OriginMeshYaw, 0).Quaternion());
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

	if (bCanInput && Controller)
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

			if (MoveDirection.Y > 0)
			{
				if (!AnimInstance->Montage_IsPlaying(LockOnForwardMontage))
				{
					AnimInstance->Montage_Play(LockOnForwardMontage, LockOnForwardMontage->GetPlayLength());
				}
			}

			else if (MoveDirection.Y < 0)
			{
				if (!AnimInstance->Montage_IsPlaying(LockOnBackwardMontage))
				{
					AnimInstance->Montage_Play(LockOnBackwardMontage, LockOnBackwardMontage->GetPlayLength());
				}
			}

			else if (MoveDirection.X > 0)
			{
				if(!AnimInstance->Montage_IsPlaying(LockOnRightwardMontage))
				{
					AnimInstance->Montage_Play(LockOnRightwardMontage, LockOnRightwardMontage->GetPlayLength());
				}
			}

			else if (MoveDirection.X < 0)
			{
				if (!AnimInstance->Montage_IsPlaying(LockOnLeftwardMontage))
				{
					AnimInstance->Montage_Play(LockOnLeftwardMontage, LockOnLeftwardMontage->GetPlayLength());
				}
			}

			AddMovementInput(Direction.GetSafeNormal(), 0.5f);
		}
	}
}

void AHeroPawn::StopMove()
{
	if (LockedOnTarget && bCanInput)
	{
		AnimInstance->Montage_Stop(0.25f);
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
		SpringArm->bUsePawnControlRotation = false;
		SpringArm->bInheritYaw = false;
	}
	else
	{
		TArray<FHitResult> HitResult;

		bool bHitted = DetectEnemy(HitResult);

		if (bHitted && HitResult.Num() > 0)
		{
			LockedOnTarget = HitResult[0].GetActor();

			SpringArm->bUsePawnControlRotation = true;
			SpringArm->bInheritYaw = true;

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
			for (const FHitResult Result : HitResult)
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
	}

	if (!bCanDashing)
	{
		DashTick(DeltaTime);
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
		Input->BindAction(IA_Movement, ETriggerEvent::Completed, this, &AHeroPawn::StopMove);
		Input->BindAction(IA_Looking, ETriggerEvent::Triggered, this, &AHeroPawn::Look);
		Input->BindAction(IA_Attack, ETriggerEvent::Started, this, &AHeroPawn::Attack);
		Input->BindAction(IA_LockOn, ETriggerEvent::Started, this, &AHeroPawn::LockOnTarget);
		Input->BindAction(IA_ChangeLockOn, ETriggerEvent::Started, this, &AHeroPawn::ChangeLockOn);
	}
}