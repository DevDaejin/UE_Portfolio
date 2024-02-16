#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HeroPawn.generated.h"

UCLASS()
class UE_PORTFOLIO_API AHeroPawn : public ACharacter
{
	GENERATED_BODY()

public:
	AHeroPawn();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
	virtual void Jump() override;
	virtual void StopJumping() override;
	virtual void Landed(const FHitResult& Hit) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxJumpCount = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxDashCount = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DashSpeed = 6000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DashDuriation = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DashCooldown = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float bCanDash = true;
private:
	UPROPERTY(VisibleAnywhere)
	int32 CurrentJumpCount = 0;

	UPROPERTY(VisibleAnywhere)
	int32 CurrentDashCount = 0;

	FTimerHandle DashTimerHanlde;
	FTimerHandle DashCooldownTimerHandle;

	void Dash();
	void ResetDash();
	void ResetDashCooldown();
};
