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
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> TargetWidget;
	UWidgetComponent* TargetWidgetComponent;

	//Animation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* LockOnForwardMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* LockOnBackwardMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* LockOnLeftwardMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* LockOnRightwardMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* DashMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control")
	float DashSpeed = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float InvincibleStartTime = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float InvincibleEndTime = 0.8f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control")
	bool bCanInput = true;

	AActor* LockedOnTarget;
	FVector MoveDirection;

	USpringArmComponent* SpringArm;
	UCameraComponent* Camera;

protected:
	virtual void BeginPlay() override;
	virtual void Jump() override;
	virtual void Landed(const FHitResult& Hit) override;
	virtual void Attack() override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void UpdateStamina();
	void LostStamina(float amount);
	void EarnStamina(float amount);
	void Dash();
	void DashTick(float DeltaTime);
	void ResetDashing();
	void ChargeStamina();
	void Move(const FInputActionInstance& Instance);
	void StopMove();
	void Look(const FInputActionInstance& Instance);
	void LockOnTarget(const FInputActionInstance& Instance);
	void ChangeLockOn(const FInputActionInstance& Instance);
	bool DetectEnemy(TArray<FHitResult>& HitResult);

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

	//Stat
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float MaxStamina = 100;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stat")
	float CurrentStamina = 100;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stat")
	float StaminaChargeAmount = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control")
	float RechargeTime = 2.f;

	//Control
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control")
	float MoveAmount = 1000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control")
	float MoveRotationSpeed = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control")
	float LockOnRotationSpeed = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control")
	float LookingSenstive = 50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control")
	float CamSpeedByLockOn = 50;

	//Jump
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control")
	int32 MaxJumpCount = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control")
	float SecondJumpForce = 600.f;

	//Stamina
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control")
	float AttackStaminaCost = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control")
	float JumpStaminaCost = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control")
	float DashStaminaCost = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control")
	bool bChargeStamina = true;

	//LockOn
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control")
	float DetectRadius = 2000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control")
	float DetectDistance = 6000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control")
	bool bCanDashing = true;


private:
	void SetInputSubsystem();
	void SetLockedOnTargetHPBar(bool bIsAct = true);
	UPROPERTY(VisibleAnywhere, Category = "Status")
	int32 CurrentJumpCount = 0;

	FTimerHandle StaminaChargeTimeHandle;
	FTimerHandle DashCooldownTimeHanlde;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	float MinPitchAngle = -85;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	float MaxPitchAngle = 85;
	float DashTickTimer;
	float OriginMeshYaw;
	FVector DashDirection;

	FRotator SpringArmOriginRotation;
	
	UAnimInstance* AnimInstance;
	UGameUI* GameUI;
};