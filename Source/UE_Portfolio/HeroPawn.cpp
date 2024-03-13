#include "HeroPawn.h"
#include "HealthComponent.h"
#include "AttackComponent.h"
#include "MainPlayerController.h"
#include "GameUI.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/SpringArmComponent.h"

AHeroPawn::AHeroPawn()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AHeroPawn::BeginPlay()
{
	Super::BeginPlay();

	SpringArm = FindComponentByClass<USpringArmComponent>();
	if (SpringArm)
	{
		SpringArmOriginRotation = SpringArm->GetRelativeRotation();
	}

	Camera = FindComponentByClass<UCameraComponent>();
	if (Camera)
	{
		AnimInstance = GetMesh()->GetAnimInstance();
	}

	SetInputSubsystem();

	CurrentStamina = MaxStamina;
	OriginMeshYaw = GetMesh()->GetRelativeRotation().Yaw;

	GameUI = Cast<AMainPlayerController>(Controller)->GameUI;

	if (HealthComponen && GameUI)
	{
		HealthComponen->OnHPChanged.AddDynamic(GameUI, &UGameUI::UpdateHPBar);
		HealthComponen->Full();
	}

	EarnStamina(MaxStamina);

	UClass* WidgetClass = TargetWidget;
	if (WidgetClass)
	{
		UUserWidget* WidgetInstance = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);
		if (WidgetInstance)
		{
			TargetWidgetComponent = NewObject<UWidgetComponent>(this);
			if (TargetWidgetComponent)
			{
				TargetWidgetComponent->SetWidget(WidgetInstance);
				TargetWidgetComponent->RegisterComponent();
				TargetWidgetComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
				TargetWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
				TargetWidgetComponent->SetVisibility(false);

				TargetWidgetComponent->SetRelativeLocation(FVector(0, 0, 130));
				UE_LOG(LogTemp, Display, TEXT("123"));
			}
		}
	}
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

void AHeroPawn::UpdateStamina()
{
	GameUI->UpdateStaminaBar(CurrentStamina / MaxStamina);
}

void AHeroPawn::LostStamina(float amount)
{
	CurrentStamina -= amount;

	if (CurrentStamina < 0)
	{
		CurrentStamina = 0;
	}

	bChargeStamina = false;

	GetWorld()->GetTimerManager().ClearTimer(StaminaChargeTimeHandle);

	GetWorld()->GetTimerManager().SetTimer(
		StaminaChargeTimeHandle,
		this,
		&AHeroPawn::ChargeStamina,
		RechargeTime);

	UpdateStamina();
}

void AHeroPawn::EarnStamina(float amount)
{
	CurrentStamina += amount;

	if (CurrentStamina > MaxStamina)
	{
		CurrentStamina = MaxStamina;
	}

	UpdateStamina();
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
					UGameplayStatics::GetWorldDeltaSeconds(this), MoveRotationSpeed);

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
				if (!AnimInstance->Montage_IsPlaying(LockOnRightwardMontage))
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

		if (SpringArm)
		{
			SpringArm->AddRelativeRotation(GetActorRotation());
			SpringArm->bUsePawnControlRotation = false;
			SpringArm->bInheritYaw = false;
		}

		if (TargetWidgetComponent)
		{
			TargetWidgetComponent->SetVisibility(false);
		}
	}
	else
	{
		TArray<FHitResult> HitResults;

		bool bHitted = DetectEnemy(HitResults);

		if (bHitted && HitResults.Num() > 0)
		{
			float ClosestDistanceSqr = TNumericLimits<float>::Max();
			ACharacterBase* ClosestTarget = nullptr;

			for (const FHitResult& Result : HitResults)
			{
				ACharacterBase* Target = Cast<ACharacterBase>(Result.GetActor());

				if (Target)
				{
					float DistanceSqr = (Target->GetActorLocation() - Camera->GetComponentLocation()).SizeSquared();
					if (DistanceSqr < ClosestDistanceSqr)
					{
						ClosestDistanceSqr = DistanceSqr;
						ClosestTarget = Target;
					}
				}
			}
			if (ClosestTarget)
			{
				SpringArm->bUsePawnControlRotation = true;
				SpringArm->bInheritYaw = true;
				TargetWidgetComponent->SetVisibility(true);
				LockedOnTarget = ClosestTarget;
			}
		}
	}
}

void AHeroPawn::ChangeLockOn(const FInputActionInstance& Instance)
{
	if (LockedOnTarget)
	{
		TArray<FHitResult> HitResults;

		bool bHitted = DetectEnemy(HitResults);

		UE_LOG(LogTemp, Display, TEXT("NUM : %d"), HitResults.Num());
		for (const FHitResult Result : HitResults)
		{
			UE_LOG(LogTemp, Display, TEXT("%s"), *Result.GetActor()->GetFName().ToString());
		}

		if (bHitted && HitResults.Num() > 0)
		{
			TArray<ACharacterBase*> CharacterBaseArray;

			for (const FHitResult& Result : HitResults)
			{
				ACharacterBase* CharacterBase = Cast<ACharacterBase>(Result.GetActor());
				if (CharacterBase && (CharacterBase != LockedOnTarget))
				{
					CharacterBaseArray.Add(CharacterBase);
				}
			}

			if (CharacterBaseArray.Num() > 0)
			{
				int32 IndexChange = (Instance.GetValue().Get<FVector>().Z > 0) ? 1 : -1;
				int32 CurrentTargetIndex = CharacterBaseArray.IndexOfByKey(LockedOnTarget);
				int32 NextTargetIndex = (CurrentTargetIndex + IndexChange) % CharacterBaseArray.Num();
				if (NextTargetIndex < 0)
				{
					NextTargetIndex += CharacterBaseArray.Num();
				}

				LockedOnTarget = CharacterBaseArray[NextTargetIndex];
				UE_LOG(LogTemp, Display, TEXT("New Lock-On Target: %s"), *LockedOnTarget->GetFName().ToString());
			}
		}
	}
}

bool AHeroPawn::DetectEnemy(TArray<FHitResult>& HitResult)
{
	FVector Start = Camera->GetComponentLocation();
	FVector End = Start + (Camera->GetForwardVector() * DetectDistance);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	DrawDebugSphere(GetWorld(), Start, DetectRadius, 24, FColor::Yellow, false, 5.0f);
	DrawDebugLine(GetWorld(), Start, End, FColor::Yellow, false, 5.0f);
	DrawDebugSphere(GetWorld(), End, DetectRadius, 24, FColor::Yellow, false, 5.0f);

	bool bHitted = GetWorld()->SweepMultiByChannel(
		HitResult,
		Start,
		End,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(DetectRadius),
		Params);

	if (bHitted)
	{
		for (FHitResult R : HitResult)
		{
			UE_LOG(LogTemp, Display, TEXT("Hit, %s"), *R.GetActor()->GetFName().ToString());
		}
	}

	return bHitted;
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
		FVector Direction = LockedOnTarget->GetActorLocation() - GetActorLocation();
		FRotator TargetRotation = Direction.Rotation();
		FRotator CurrentRotation = Controller->GetControlRotation();
		FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, LockOnRotationSpeed);
		Controller->SetControlRotation(NewRotation);

		FRotator NewSpringArmRotation = FMath::RInterpTo(SpringArm->GetRelativeRotation(), FRotator(-30, 0, 0), DeltaTime, LockOnRotationSpeed);
		SpringArm->SetRelativeRotation(NewSpringArmRotation);

		TargetWidgetComponent->SetWorldLocation(LockedOnTarget->GetActorLocation());
		DrawDebugBox(GetWorld(), LockedOnTarget->GetActorLocation(), FVector(100, 100, 100), FColor::Blue);
	}

	if (!bCanDashing)
	{
		DashTick(DeltaTime);
	}

	UE_LOG(LogTemp, Display, TEXT("SpringArm %s"), *SpringArm->GetRelativeRotation().ToString());
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