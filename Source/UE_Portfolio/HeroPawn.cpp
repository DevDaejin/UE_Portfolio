#include "HeroPawn.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

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

	CurrentHP = MaxHP;
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
			return;

		CurrentJumpCount++;
	}
}

void AHeroPawn::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	CurrentJumpCount = 0;
}

float AHeroPawn::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (Damage > 0.f)
	{
		LostHP(Damage);
	}
	return 0.0f;
}

void AHeroPawn::LostHP(float amount)
{
	CurrentHP -= amount;

	if (CurrentHP <= 0.f)
	{
		Death();
	}
}

void AHeroPawn::EarnHP(float amount)
{
	CurrentHP += amount;

	if (CurrentHP > MaxHP)
	{
		CurrentHP = MaxHP;
	}
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

void AHeroPawn::Death()
{
	USkeletalMeshComponent* MeshComponent = GetMesh();

	if (MeshComponent)
	{
		MeshComponent->SetCollisionProfileName(TEXT("Ragdoll"));
		MeshComponent->SetSimulatePhysics(true);
		MeshComponent->SetAllBodiesSimulatePhysics(true);
		MeshComponent->WakeAllRigidBodies();

		UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
		if (MovementComponent)
		{
			MovementComponent->StopMovementImmediately();
			MovementComponent->DisableMovement();
			MovementComponent->SetComponentTickEnabled(false);
		}
	}
}

void AHeroPawn::Dash()
{
	if (!bLockMovement && bCanDash)
	{
		FVector Direction = Controller->GetPawn()->GetActorForwardVector();
		LaunchCharacter(Direction * DashSpeed, true, true);
		AnimInstance->Montage_Play(DashMontage, DashMontage->GetPlayLength());

		bLockMovement = true;
		GetWorld()->GetTimerManager().SetTimer(LockMovementTimerHandle, this, &AHeroPawn::ReleaseMovement, DashMontage->GetPlayLength() * 0.8f);

		LostStamina(DashStaminaCost);
		
		GetWorld()->GetTimerManager().SetTimer(DashCoolTimerHandle, this, &AHeroPawn::ResetDashCoolTime, DashCoolTime);

		UE_LOG(LogTemp, Display, TEXT("대쉬 무적 설정"));
		GetWorld()->GetTimerManager().SetTimer(DashInvicibleTimerHanlde, this, &AHeroPawn::ResetDashInvicible, DashInvincibleTime);

		bCanDash = false;
	}
}

void AHeroPawn::ChargeStamina()
{
	bChargeStamina = true;
}

void AHeroPawn::ResetDashInvicible()
{
	UE_LOG(LogTemp, Display, TEXT("대쉬 무적 해제"));
}

void AHeroPawn::ResetDashCoolTime()
{
	bCanDash = true;
}

void AHeroPawn::Attack()
{
	if (AnimInstance && AttackMontage)
	{
		bLockMovement = true;
		AnimInstance->Montage_Play(AttackMontage, 1.f);

		GetWorld()->GetTimerManager().SetTimer(LockMovementTimerHandle, this, &AHeroPawn::ReleaseMovement, AttackMontage->GetPlayLength());
	}
}

void AHeroPawn::ReleaseMovement()
{
	bLockMovement = false;
}

void AHeroPawn::Move(const FInputActionInstance& Instance)
{
	if (!bLockMovement)
	{
		MoveDirection = FVector(Instance.GetValue().Get<FVector2D>().GetSafeNormal().X, Instance.GetValue().Get<FVector2D>().GetSafeNormal().Y, 0);

		if (Controller)
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

// Called every frame
void AHeroPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bChargeStamina && bCanDash)
	{
		EarnStamina(StaminaChargeAmount * DeltaTime);
	}
}

// Called to bind functionality to input
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