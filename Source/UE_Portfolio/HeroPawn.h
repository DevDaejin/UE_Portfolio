#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "CharacterBase.h"
#include "HeroPawn.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UWidgetComponent;
class UGameUI;

UCLASS()
class UE_PORTFOLIO_API AHeroPawn : public ACharacterBase
{
	GENERATED_BODY()

public:
	AHeroPawn();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Jump() override;
	virtual void Landed(const FHitResult& Hit) override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

//Input
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* CommonMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_Jump;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_Dash;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_Movement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_Looking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_LockOn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_ChangeLockOn;

//Animation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* DashMontage;

//LockOn
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> TargetWidget;
	UWidgetComponent* TargetWidgetComponent;

public:
	bool bCanInput = true;
	AActor* LockedOnTarget;
	FVector MoveDirection;
	UCameraComponent* GetCamera();

private:
//Movement
	float MoveAmount = 1000;
	float MoveRotationSpeed = 10;

	void Move(const FInputActionInstance& Instance);
	void StopMove();

//Camera
	USpringArmComponent* SpringArm;
	UCameraComponent* Camera;
	float LockOnRotationSpeed = 10;
	float LookingSenstive = 50;
	float CamSpeedByLockOn = 50;
	float MinPitchAngle = -85;
	float MaxPitchAngle = 85;
	FRotator SpringArmOriginRotation;

	void Look(const FInputActionInstance& Instance);

//Jump
	int32 MaxJumpCount = 2;
	float SecondJumpForce = 600.f;
	float JumpStaminaCost = 10;
	int32 CurrentJumpCount = 0;

//Dash
	bool bCanDashing = true;
	float DashStaminaCost = 10;
	float DashSpeed = 500.f;
	float InvincibleStartTime = 0.2f;
	float InvincibleEndTime = 0.8f;
	FTimerHandle DashCooldownTimeHanlde;
	float DashTickTimer;
	float OriginMeshYaw;
	FVector DashDirection;

	void Dash();
	void DashTick(float DeltaTime);
	void ResetDashing();

//Attack
	float AttackStaminaCost = 10;
	float AttackChargingTime = 0;
	float ThresholdAttackChargingTime = 1.f;
	float bIsOverCharging = false;

	void AttackPressed();
	void AttackPressing();
	void AttackReleased();

//Statmina
	float MaxStamina = 100;
	float CurrentStamina = 100;
	float StaminaChargeAmount = 10.f;
	float RechargeTime = 2.f;
	bool bChargeStamina = true;
	FTimerHandle StaminaChargeTimeHandle;

	void UpdateStamina();
	void LostStamina(float amount);
	void EarnStamina(float amount);
	void ChargeStamina();

//LockOn
	UGameUI* GameUI;
	float DetectRadius = 2000;
	float DetectDistance = 6000.f;
	void SetLockedOnTargetHPBar(bool bIsAct = true);

	void LockOnTarget(const FInputActionInstance& Instance);
	void ChangeLockOn(const FInputActionInstance& Instance);
	bool DetectEnemy(TArray<FHitResult>& HitResult);

//Input
	void SetInputSubsystem();

//Animation
	UAnimInstance* AnimInstance;
};