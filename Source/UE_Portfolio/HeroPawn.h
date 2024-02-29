#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "CharacterBase.h"
#include "HeroPawn.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class UE_PORTFOLIO_API AHeroPawn : public ACharacterBase
{
	GENERATED_BODY()

public:
	AHeroPawn();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Animation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* DashMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control")
	bool bLockMovement = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control")
	float DashSpeed = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float InvincibleStartTime = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float InvincibleEndTime = 0.8f;

protected:
	virtual void BeginPlay() override;
	virtual void Jump() override;
	virtual void Landed(const FHitResult& Hit) override;
	virtual void Attack() override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void LostStamina(float amount);
	void EarnStamina(float amount);
	void Dash();
	void ChargeStamina();
	void Move(const FInputActionInstance& Instance);
	void Look(const FInputActionInstance& Instance);
	void LockOn(const FInputActionInstance& Instance);
	void ChangeLockOn(const FInputActionInstance& Instance);

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float MaxStamina = 100;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stat")
	float CurrentStamina = 100;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stat")
	float StaminaChargeAmount = .0001f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control")
	float RechargeTime = 1.f;

	//Control
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control")
	float MoveAmount = 1000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control")
	float RotateSpeed = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control")
	float LookingSenstive = 50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control")
	int32 MaxJumpCount = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control")
	float SecondJumpForce = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control")
	float AttackStaminaCost = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control")
	float JumpStaminaCost = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control")
	float DashStaminaCost = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control")
	bool bChargeStamina = true;

private:
	void SetInputSubsystem();

	UPROPERTY(VisibleAnywhere, Category = "Status")
	int32 CurrentJumpCount = 0;

	FVector MoveDirection;
	FTimerHandle StaminaChargeTimeHandle;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	float MinPitchAngle = -85;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	float MaxPitchAngle = 85;

	USpringArmComponent* SpringArm;
	UCameraComponent* Camera;
	UAnimInstance* AnimInstance;
};