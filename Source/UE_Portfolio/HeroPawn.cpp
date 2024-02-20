// Fill out your copyright notice in the Description page of Project Settings.


#include "HeroPawn.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
AHeroPawn::AHeroPawn()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AHeroPawn::BeginPlay()
{
	Super::BeginPlay();

	SpringArm = FindComponentByClass<USpringArmComponent>();
	Camera = FindComponentByClass<UCameraComponent>();

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
			else
			{
				UE_LOG(LogTemp, Display, TEXT("Subsystem is nullptr"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("LocalPlayer is nullptr"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerController is nullptr"));
	}
}

void AHeroPawn::Jump()
{
	if (CurrentJumpCount == 0)
	{
		Super::Jump();
	}
	else if (CurrentJumpCount == 1)
	{
		FVector Direction = GetActorUpVector();
		LaunchCharacter(Direction * SecondJumpForce, false, true);
	}
	else
		return;

	CurrentJumpCount++;
}
void AHeroPawn::StopJumping()
{
	Super::StopJumping();
}

void AHeroPawn::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	CurrentJumpCount = 0;
}

void AHeroPawn::Dash()
{
	if (bCanDash)
	{
		FVector Direction = GetActorForwardVector();
		LaunchCharacter(Direction * DashSpeed, true, true);
		CurrentDashCount++;

		if (CurrentDashCount < MaxDashCount && bCanDash)
		{
			bCanDash = false;
			GetWorld()->GetTimerManager().SetTimer(DashCooldownTimerHandle, this, &AHeroPawn::ResetDashCooldown, DashCooldown);
		}

		GetWorld()->GetTimerManager().SetTimer(DashTimerHanlde, this, &AHeroPawn::ResetDash, DashDuriation);
	}
}

void AHeroPawn::ResetDash()
{
	CurrentDashCount = 0;

	if (!bCanDash)
	{
		GetWorld()->GetTimerManager().ClearTimer(DashCooldownTimerHandle);
		bCanDash = true;
	}
}

void AHeroPawn::ResetDashCooldown()
{
	bCanDash = true;
}

void AHeroPawn::Move(const FInputActionInstance& Instance)
{
	MoveDirection = Instance.GetValue().Get<FVector2D>().GetSafeNormal();

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
	}
}

