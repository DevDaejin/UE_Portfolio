// Fill out your copyright notice in the Description page of Project Settings.


#include "HeroPawn.h"
#include "EnhancedInputComponent.h"
#include "InputmappingContext.h"

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

}

void AHeroPawn::Jump()
{
	if (CurrentJumpCount < MaxJumpCount)
	{
		Super::Jump();
		CurrentJumpCount++;
	}
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

// Called every frame
void AHeroPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AHeroPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

