#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "EnemyBase.generated.h"

class UWidgetComponent;

UCLASS()
class UE_PORTFOLIO_API AEnemyBase : public ACharacterBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	AActor* AA;
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	UMeshComponent* HPWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	UWidgetComponent* TargetWidget;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
private:
	void Billboarding();
};
