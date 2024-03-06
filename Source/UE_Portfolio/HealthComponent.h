#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

class UHPBar;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_PORTFOLIO_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHealthComponent();

protected:
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> HPBarSubclass;
	UHPBar* HPBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 HP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxHP;

	int32 GetCurrentHP();

	void EarnHP(int Amount);
	bool LostHP(int Amount);
	void EarnMaxHP(int Amount);
	void LostMaxHP(int Amount);
	void UpdateHPBar();
	void Kill();
	void Full();
};
