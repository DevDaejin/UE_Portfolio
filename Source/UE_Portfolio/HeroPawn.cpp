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

AHeroPawn::AHeroPawn()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AHeroPawn::BeginPlay()
{
	Super::BeginPlay();

	SpringArm = FindComponentByClass<USpringArmComponent>();
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
	GetWorld()->GetTimerManager().SetTimer(StaminaChargeTimeHandle, this, &AHeroPawn::ChargeStamina, RechargeTime);
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
	Super::Attack();

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
	MoveDirection = FVector(Instance.GetValue().Get<FVector2D>().GetSafeNormal().X, Instance.GetValue().Get<FVector2D>().GetSafeNormal().Y, 0);

	if (!bLockMovement && Controller)
	{
		FRotator CameraRotator = SpringArm->GetRelativeRotation();
		CameraRotator.Pitch = 0;
		CameraRotator.Roll = 0;

		FVector ForwardDirection = FRotationMatrix(CameraRotator).GetUnitAxis(EAxis::X);
		FVector RightDirection = FRotationMatrix(CameraRotator).GetUnitAxis(EAxis::Y);
		FVector Direction = ForwardDirection * MoveDirection.Y + RightDirection * MoveDirection.X;
		if (!Direction.IsNearlyZero())
		{
			FRotator CalculatedRotation = FMath::RInterpTo(Controller->GetControlRotation(), Direction.Rotation(), UGameplayStatics::GetWorldDeltaSeconds(this), RotateSpeed);
			Controller->SetControlRotation(CalculatedRotation);
		}

		AddMovementInput(Direction.GetSafeNormal(), 1);
	}
}

void AHeroPawn::Look(const FInputActionInstance& Instance)
{
	FVector2D LookVector = Instance.GetValue().Get<FVector2D>();
	LookVector.X *= (UGameplayStatics::GetWorldDeltaSeconds(this) * LookingSenstive);
	LookVector.Y *= (UGameplayStatics::GetWorldDeltaSeconds(this) * LookingSenstive);

	FRotator CalculatedSpringArmRotation = SpringArm->GetComponentRotation();
	CalculatedSpringArmRotation.Yaw += LookVector.X;
	CalculatedSpringArmRotation.Pitch -= LookVector.Y;
	CalculatedSpringArmRotation.Pitch = FMath::Clamp(CalculatedSpringArmRotation.Pitch, MinPitchAngle, MaxPitchAngle);
	SpringArm->SetWorldRotation(CalculatedSpringArmRotation);
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
	}
}