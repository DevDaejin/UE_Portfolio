#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "HeroPawn.generated.h"

class AWeapon;
class USpringArmComponent;
class UCameraComponent;
class UHealthComponent;

UCLASS()
class UE_PORTFOLIO_API AHeroPawn : public ACharacter
{
	GENERATED_BODY()

public:
	AHeroPawn();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool bIsAttacking;

	UHealthComponent* HealthComponent;

protected:

	virtual void BeginPlay() override;
	virtual void Jump() override;
	virtual void Landed(const FHitResult& Hit) override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void LostHP(float amount);
	void EarnHP(float amount);
	void LostStamina(float amount);
	void EarnStamina(float amount);
	void Death();
	void Dash();
	void ChargeStamina();
	void ResetDashInvicible();
	void ResetDashCoolTime();
	void ReleaseMovement();
	void Arm();
	void Move(const FInputActionInstance& Instance);
	void Look(const FInputActionInstance& Instance);
	void Attack();
	void CheckWeaponCollision();

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

	//Animation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* DashMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* AttackMontage;

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
	float JumpStaminaCost = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control")
	float DashStaminaCost = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control")
	float DashSpeed = 3000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control")
	float DashCoolTime = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control")
	float DashInvincibleTime = .5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control")
	bool bCanDash = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control")
	bool bChargeStamina = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control")
	bool bLockMovement = false;

	//ETC
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ETC")
	TSubclassOf<AWeapon> WeaponSubclass;
	AWeapon* Weapon;

private:
	void SetInputSubsystem();

	UPROPERTY(VisibleAnywhere, Category = "Status")
	int32 CurrentJumpCount = 0;

	FVector MoveDirection;
	FTimerHandle StaminaChargeTimeHandle;
	FTimerHandle DashCoolTimerHandle;
	FTimerHandle DashInvicibleTimerHanlde;
	FTimerHandle LockMovementTimerHandle;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	float MinPitchAngle = -85;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	float MaxPitchAngle = 85;

	USpringArmComponent* SpringArm;
	UCameraComponent* Camera;
	UAnimInstance* AnimInstance;
};