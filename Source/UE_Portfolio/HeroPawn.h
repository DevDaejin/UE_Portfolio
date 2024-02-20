#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "HeroPawn.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class UE_PORTFOLIO_API AHeroPawn : public ACharacter
{
	GENERATED_BODY()

public:
	AHeroPawn();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	virtual void BeginPlay() override;
	virtual void Jump() override;
	virtual void StopJumping() override;
	virtual void Landed(const FHitResult& Hit) override;

	void Dash();
	void ResetDash();
	void ResetDashCooldown();
	void Move(const FInputActionInstance& Instance);
	void Look(const FInputActionInstance& Instance);

	//Input
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputMappingContext* CommonMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* IA_Jump;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* IA_Dash;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* IA_Movement;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_Looking;

	//Status
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float MoveAmount = 1000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float RotateSpeed = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float LookingSenstive = 50;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Status")
	int32 MaxJumpCount = 2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Status")
	int32 MaxDashCount = 2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float SecondJumpForce = 600.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Status")
	float DashSpeed = 600.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Status")
	float DashDuriation = 0.2f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Status")
	float DashCooldown = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Status")
	bool bCanDash = true;

private:
	UPROPERTY(VisibleAnywhere, Category = "Status")
	int32 CurrentJumpCount = 0;

	UPROPERTY(VisibleAnywhere, Category = "Status")
	int32 CurrentDashCount = 0;

	FVector2D MoveDirection;
	FTimerHandle DashTimerHanlde;
	FTimerHandle DashCooldownTimerHandle;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	float MinPitchAngle = -85;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	float MaxPitchAngle = 85;

	USpringArmComponent* SpringArm;
	UCameraComponent* Camera;
};
