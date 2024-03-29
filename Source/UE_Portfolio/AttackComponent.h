
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttackComponent.generated.h"

class AWeapon;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_PORTFOLIO_API UAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAttackComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void Attack();
	void CheckWeaponCollision();
	bool bIsAttacking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* AttackMontage;

private:
	TArray<AActor*> Enemies;
	TArray<FVector> OldFVectors;
	TArray<FVector> CurrentFVectors;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	int WeaponTraceSegment = 1;
	int MinWeaponTraceSegment = 2;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float Damage = 10;
};
